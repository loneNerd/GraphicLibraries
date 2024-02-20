#pragma once
#ifndef ENGINE_EDITOR_HPP_
#define ENGINE_EDITOR_HPP_

#include "context.hpp"

namespace Engine
{
    namespace Panels { class SceneView; }

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
        void setupUI();

        uint64_t m_elapsedFrames = 0;
        Context& m_context;
        std::shared_ptr<Panels::SceneView> m_sceneView = nullptr;
    };
}

#endif // ENGINE_EDITOR_HPP_
