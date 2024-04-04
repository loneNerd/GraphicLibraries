#pragma once
#ifndef ENGINE_PANELS_BASE_VIEW_HPP_
#define ENGINE_PANELS_BASE_VIEW_HPP_

#include <memory>

#include "math/quaternion.hpp"
#include "ui/panels/panel_window.hpp"

namespace Engine::OpenGL { class Camera; }

namespace Engine::Panels
{
    class BaseView : public UI::Panels::PanelWindow
    {
    public:
        BaseView
        (
            const std::string& title,
            bool opened,
            const UI::Settings::PanelWindowSettings& windowSettings
        );

        virtual ~BaseView();

        BaseView(const BaseView& other)             = delete;
        BaseView(BaseView&& other)                  = delete;
        BaseView& operator=(const BaseView& other)  = delete;
        BaseView& operator=(const BaseView&& other) = delete;

        virtual void Update(float dt) { }
        void DrawImpl() override;
        virtual void RenderImpl() = 0;
        void Render();
        void SetCameraPosition(const Math::FVector3 position);
        void SetCameraRotation(const Math::FQuaternion rotation);
        const Math::FVector3& GetCameraPosition() const;
        const Math::FQuaternion& GetCameraRotation() const;
        const std::shared_ptr<Engine::OpenGL::Camera> GetCamera() const;

        std::pair<uint16_t, uint16_t> GetSafeSize() const;

    protected:
        void prepareCamera();

        std::shared_ptr<Engine::OpenGL::Camera> m_camera = nullptr;
        Engine::Math::FVector3 m_cameraPosition = { 0.0f, 0.0f, 5.0f };
        Engine::Math::FQuaternion m_cameraRotation = Engine::Math::FQuaternion({ 0.0f, 0.0f, 0.0f });
    };
}

#endif // ENGINE_PANELS_BASE_VIEW_HPP_
