#pragma once
#ifndef ENGINE_EDITOR_CONTEXT_HPP_
#define ENGINE_EDITOR_CONTEXT_HPP_

#include <filesystem>
#include <memory>
#include <string>

#include "core/resource_management/material_manager.hpp"
#include "core/resource_management/model_manager.hpp"
#include "core/resource_management/shader_manager.hpp"
#include "core/resource_management/texture_manager.hpp"
#include "core/scene_system/scene_manager.hpp"
#include "glfw/settings/window_settings.hpp"

namespace Engine
{

namespace Core   { class Renderer; }
namespace OpenGL { class Driver; }

namespace GLFW
{
    class GLFW;
    class Device;
}

namespace Editor
{
    class Context
    {
    public:
        Context(const std::filesystem::path& projectPath);
        ~Context();

        Context(const Context& other)             = delete;
        Context(Context&& other)                  = delete;
        Context& operator=(const Context& other)  = delete;
        Context& operator=(const Context&& other) = delete;

        const std::filesystem::path ProjectPath;
        const std::filesystem::path ProjectAssetsPath;
        const std::filesystem::path ProjectShadersPath;

        std::unique_ptr<GLFW::Device>   Device;
        std::unique_ptr<GLFW::GLFW>     Window;
        std::shared_ptr<OpenGL::Driver> Driver;
        std::unique_ptr<Core::Renderer> Renderer;

        Core::SceneSystem::SceneManager SceneManager;

        GLFW::Settings::WindowSettings WindowSettings;
    };
}
}

#endif // ENGINE_EDITOR_CONTEXT_HPP_
