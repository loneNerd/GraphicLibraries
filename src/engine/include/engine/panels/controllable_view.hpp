#pragma once
#ifndef ENGINE_PANELS_CONTROLLABLE_VIEW_HPP_
#define ENGINE_PANELS_CONTROLLABLE_VIEW_HPP_

#include "engine/panels/base_view.hpp"
#include "engine/utils/camera_controller.hpp"

namespace Engine::Panels
{
    class ControllableView : public BaseView
    {
    public:
        ControllableView
        (
            const std::string& title,
            bool opened,
            const UI::Settings::PanelWindowSettings& windowSettings,
            bool enableFocusInput = false
        );

        virtual ~ControllableView();

        ControllableView(const ControllableView& other)             = delete;
        ControllableView(ControllableView&& other)                  = delete;
        ControllableView& operator=(const ControllableView& other)  = delete;
        ControllableView& operator=(const ControllableView&& other) = delete;

        virtual void Update(float dt) override;
        const std::shared_ptr<Engine::Utils::CameraController> GetCameraController() const;

    protected:
        std::shared_ptr<Engine::Utils::CameraController> m_cameraController;
    };
}

#endif // ENGINE_PANELS_CONTROLLABLE_VIEW_HPP_
