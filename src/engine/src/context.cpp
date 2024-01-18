#include "engine/context.hpp"

#include "core/renderer.hpp"
#include "opengl/driver.hpp"
#include "windows/sdl2.hpp"

namespace Editor = Engine::Editor;

Editor::Context::Context(const std::filesystem::path& projectPath)
    : ProjectPath(projectPath),
      ProjectAssetsPath(projectPath / "assets\\"),
      ProjectShadersPath(projectPath / "opengl\\shaders\\"),
      SceneManager(ProjectAssetsPath / "scenes\\")
{
    Core::ResourceManagement::ModelManager::SetAssetPaths(ProjectAssetsPath);
    Core::ResourceManagement::TextureManager::SetAssetPaths(ProjectAssetsPath);
    Core::ResourceManagement::ShaderManager::SetAssetPaths(ProjectShadersPath);
    Core::ResourceManagement::MaterialManager::SetAssetPaths(ProjectAssetsPath);

    Windows::Settings::WindowSettings windowSettings;
    windowSettings.Title = "Editor";
    windowSettings.Width = 1280;
    windowSettings.Height = 720;
    windowSettings.Flags = Windows::Settings::ESDL2WindowFlags::OpenGL     |
                           Windows::Settings::ESDL2WindowFlags::InputFocus |
                           Windows::Settings::ESDL2WindowFlags::Shown;

    Window = std::make_shared<Windows::SDL2>(windowSettings);
    Window->SetTitle("Engine");

    Driver = std::make_shared<Engine::OpenGL::Driver>(true);
    Renderer = std::make_unique<Core::Renderer>(Driver);
    Renderer->SetCapability(OpenGL::Settings::ERenderingCapability::Multisample, true);
}

Editor::Context::~Context()
{
    Core::ResourceManagement::ModelManager::Instance().RemoveResources();
    Core::ResourceManagement::TextureManager::Instance().RemoveResources();
    Core::ResourceManagement::ShaderManager::Instance().RemoveResources();
    Core::ResourceManagement::MaterialManager::Instance().RemoveResources();
}
