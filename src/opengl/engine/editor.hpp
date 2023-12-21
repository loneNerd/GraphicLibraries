#pragma once
#ifndef ENGINE_EDITOR_EDITOR_HPP_
#define ENGINE_EDITOR_EDITOR_HPP_

#include "context.hpp"

namespace Engine::OpenGL { class Camera; }

namespace Engine
{
namespace Editor
{
    class Editor
    {
    public:
        Editor(Context& context);
        ~Editor();

        Editor(const Editor& other)             = delete;
        Editor(Editor&& other)                  = delete;
        Editor& operator=(const Editor& other)  = delete;
        Editor& operator=(const Editor&& other) = delete;

        void PreUpdate();
        void Update(float dt);
        void PostUpdate();

    private:
        uint64_t m_elapsedFrames = 0;
        Context& m_context;
        std::shared_ptr<Engine::OpenGL::Resources::Shader> m_shader = nullptr;
        std::shared_ptr<Engine::OpenGL::Camera> m_camera = nullptr;
        Engine::Math::FVector3 m_cameraPosition = { 0.0f, 0.0f, 5.0f };
        Engine::Math::FQuaternion m_cameraRotation = Engine::Math::FQuaternion({ 0.0f, 180.0f, 0.0f });
    };
}
}

#endif // ENGINE_EDITOR_EDITOR_HPP_
