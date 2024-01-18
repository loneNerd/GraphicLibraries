#pragma once
#ifndef ENGINE_CORE_RENDERER_HPP_
#define ENGINE_CORE_RENDERER_HPP_

#include <map>

#include "core/resources/material.hpp"
#include "core/scene_system/scene.hpp"
#include "opengl/opengl.hpp"
#include "opengl/resources/texture.hpp"

namespace Engine
{
namespace Core
{
    class Renderer : public OpenGL::OpenGL
    {
    public:

        Renderer(std::shared_ptr<Engine::OpenGL::Driver> driver) : OpenGL::OpenGL(driver) { }
        ~Renderer() = default;

        Renderer(const Renderer& other)             = delete;
        Renderer(Renderer&& other)                  = delete;
        Renderer& operator=(const Renderer& other)  = delete;
        Renderer& operator=(const Renderer&& other) = delete;

        void RenderScene
        (
            std::shared_ptr<Engine::Core::SceneSystem::Scene> scene,
            std::shared_ptr<Engine::OpenGL::Camera> camera,
            const Math::FVector3& cameraPosition,
            const Math::FQuaternion& cameraRotation,
            std::shared_ptr<Engine::OpenGL::Resources::Shader> shader
        );

        void DrawMesh(std::shared_ptr<Engine::OpenGL::Resources::Mesh> mesh, std::shared_ptr<Engine::OpenGL::Resources::Shader> shader, std::shared_ptr<Engine::OpenGL::Camera> camera);
    };
}
}

#endif // ENGINE_CORE_RENDERER_HPP_
