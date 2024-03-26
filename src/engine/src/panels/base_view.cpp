#include "engine/panels/base_view.hpp"

#include "core/renderer.hpp"
#include "ui/ui_manager.hpp"
#include "opengl/camera.hpp"
#include "tools/utils/service_locator.hpp"
#include "windows/sdl2.hpp"

namespace Math = Engine::Math;
namespace OpenGL = Engine::OpenGL;
namespace Core = Engine::Core;

Engine::Panels::BaseView::BaseView
(
    const std::string& title,
    bool opened,
    const UI::Settings::PanelWindowSettings& windowSettings
) : PanelWindow(title, opened, windowSettings)
{
    m_cameraPosition = { 0.0f, 0.0f, -5.0f };
    m_cameraRotation = Math::FQuaternion({ 0.0f, 0.0f, 0.0f });

    m_camera = std::make_shared<OpenGL::Camera>();
    m_camera->SetProjectionMode(OpenGL::Settings::EProjectionMode::Perspective);

    m_windowSettings.Scrollable = false;
}

Engine::Panels::BaseView::~BaseView()
{
    if (m_camera)
        m_camera = nullptr;
}

void Engine::Panels::BaseView::DrawImpl()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    UI::Panels::PanelWindow::DrawImpl();

    ImGui::PopStyleVar();
}

void Engine::Panels::BaseView::Render()
{
    RenderImpl();
}

void Engine::Panels::BaseView::SetCameraPosition(const Math::FVector3 position)
{
    m_cameraPosition = position;
}

void Engine::Panels::BaseView::SetCameraRotation(const Math::FQuaternion rotation)
{
    m_cameraRotation = rotation;
}

const Math::FVector3& Engine::Panels::BaseView::GetCameraPosition() const
{
    return m_cameraPosition;
}

const Math::FQuaternion& Engine::Panels::BaseView::GetCameraRotation() const
{
    return m_cameraRotation;
}

const std::shared_ptr<Engine::OpenGL::Camera> Engine::Panels::BaseView::GetCamera() const
{
    return m_camera;
}

std::pair<uint16_t, uint16_t> Engine::Panels::BaseView::GetSafeSize() const
{
    std::pair<uint16_t, uint16_t> size = Tools::Utils::ServiceLocator::Get<Windows::SDL2>()->GetSize();
    //auto result = GetSize() - Math::FVector2 { 0.f, 25.f }; // 25 == title bar height
    auto result = Math::FVector2(size.first, size.second) - Math::FVector2 { 0.f, 25.f }; // 25 == title bar height
    return { static_cast<uint16_t>(result.X), static_cast<uint16_t>(result.Y) };
}

void Engine::Panels::BaseView::prepareCamera()
{
    auto [winWidth, winHeight] = GetSafeSize();
    m_camera->CacheMatrices(winWidth, winHeight, m_cameraPosition, m_cameraRotation);
}
