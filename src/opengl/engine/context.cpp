#include "context.hpp"

#include "core/renderer.hpp"
#include "opengl/driver.hpp"
#include "glfw/device.hpp"
#include "glfw/glfw.hpp"

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

    GLFW::Settings::DeviceSettings deviceSettings;
    deviceSettings.ContextMajorVersion = 4;
    deviceSettings.ContextMinorVersion = 6;
    WindowSettings.Title = "Editor";
    WindowSettings.Width = 1280;
    WindowSettings.Height = 720;

    Device = std::make_unique<GLFW::Device>(deviceSettings);
    Window = std::make_unique<GLFW::GLFW>(*Device, WindowSettings);
    Window->MakeCurrentContext();

    Device->SetVsync(true);

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
