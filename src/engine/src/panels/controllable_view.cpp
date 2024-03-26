#include "engine/panels/controllable_view.hpp"

namespace Panels = Engine::Panels;

Panels::ControllableView::ControllableView
(
    const std::string& title,
    bool opened,
    const UI::Settings::PanelWindowSettings& windowSettings,
    bool enableFocusInputs
    ) : BaseView(title, opened, windowSettings)
{
    m_cameraController = std::make_shared<Utils::CameraController>(this, m_camera, m_cameraPosition, m_cameraRotation, enableFocusInputs);
}

Panels::ControllableView::~ControllableView()
{
    if (m_cameraController)
        m_cameraController = nullptr;
}

void Panels::ControllableView::ControllableView::Update(float dt)
{
    m_cameraController->HandleInputs(dt);

    BaseView::Update(dt);
}

const std::shared_ptr<Engine::Utils::CameraController> Panels::ControllableView::ControllableView::GetCameraController() const
{
    return m_cameraController;
}
