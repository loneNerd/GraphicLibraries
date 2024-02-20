#pragma once
#ifndef ENGINE_PANELS_SCENE_VIEW_HPP_
#define ENGINE_PANELS_SCENE_VIEW_HPP_

#include "base_view.hpp"

namespace Engine::OpenGL::Resources { class Shader; }

namespace Engine::Panels
{
    class SceneView : public BaseView
    {
    public:
        SceneView
        (
            const std::string& title,
            bool opened,
            const UI::Settings::PanelWindowSettings& windowSettings
        );

        virtual ~SceneView();

        SceneView(const SceneView& other)             = delete;
        SceneView(SceneView&& other)                  = delete;
        SceneView& operator=(const SceneView& other)  = delete;
        SceneView& operator=(const SceneView&& other) = delete;

        virtual void RenderImpl() override;

    private:
        std::shared_ptr<Engine::OpenGL::Resources::Shader> m_shader = nullptr;
    };
}

#endif // ENGINE_PANELS_SCENE_VIEW_HPP_
