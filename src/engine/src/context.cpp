#include "engine/context.hpp"

#include "core/renderer.hpp"
#include "core/resource_management/material_manager.hpp"
#include "core/resource_management/model_manager.hpp"
#include "core/resource_management/shader_manager.hpp"
#include "core/resource_management/texture_manager.hpp"
#include "core/scene_system/scene_manager.hpp"
#include "opengl/driver.hpp"
#include "tools/utils/service_locator.hpp"
#include "ui/ui_manager.hpp"
#include "windows/inputs/input_manager.hpp"
#include "windows/sdl2.hpp"

#include <iostream>

Engine::Context::Context(const std::filesystem::path& projectPath)
    : ProjectPath(projectPath),
      ProjectAssetsPath(projectPath / "assets\\"),
      ProjectShadersPath(projectPath / "opengl\\shaders\\")
{
    Core::ResourceManagement::ModelManager::SetAssetPaths(ProjectAssetsPath);
    Core::ResourceManagement::TextureManager::SetAssetPaths(ProjectAssetsPath);
    Core::ResourceManagement::ShaderManager::SetAssetPaths(ProjectShadersPath);
    Core::ResourceManagement::MaterialManager::SetAssetPaths(ProjectAssetsPath);

    Windows::Settings::WindowSettings windowSettings;
    windowSettings.Title = "Editor";
    windowSettings.Width = 1280;
    windowSettings.Height = 720;
    windowSettings.Flags = Windows::Settings::ESDL2WindowFlags::OpenGL       |
                           Windows::Settings::ESDL2WindowFlags::AllowHighdpi |
                           Windows::Settings::ESDL2WindowFlags::InputFocus   |
                           Windows::Settings::ESDL2WindowFlags::Shown;

    std::shared_ptr<Windows::SDL2> window = Tools::Utils::ServiceLocator::Provide<Windows::SDL2>(windowSettings);
    window->SetTitle("Engine");

    Tools::Utils::ServiceLocator::Provide<Windows::Inputs::InputManager>(window);

    Tools::Utils::ServiceLocator::Provide<Engine::OpenGL::Driver>(true);
    Tools::Utils::ServiceLocator::Provide<Core::Renderer>(Tools::Utils::ServiceLocator::Get<Engine::OpenGL::Driver>());
    Tools::Utils::ServiceLocator::Get<Core::Renderer>()->SetCapability(OpenGL::Settings::ERenderingCapability::Multisample, true);

    std::filesystem::create_directories(projectPath / "engine");

    std::shared_ptr<UI::UIManager> uiManager = Tools::Utils::ServiceLocator::Provide<UI::UIManager>(window, UI::Styling::EStyle::AlternativeDark);
    uiManager->SetEditorLayoutAutosaveFrequency(60.0f);
    uiManager->EnableDocking(true);

    Tools::Utils::ServiceLocator::Provide<Core::SceneSystem::SceneManager>(ProjectAssetsPath / "scenes\\");
}

Engine::Context::~Context()
{
    Core::ResourceManagement::ModelManager::Instance().RemoveResources();
    Core::ResourceManagement::TextureManager::Instance().RemoveResources();
    Core::ResourceManagement::ShaderManager::Instance().RemoveResources();
    Core::ResourceManagement::MaterialManager::Instance().RemoveResources();
}
