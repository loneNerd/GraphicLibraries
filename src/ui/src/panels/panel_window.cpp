#include "ui/panels/panel_window.hpp"
#include "ui/internal/converter.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Internal = Engine::UI::Internal;
namespace Panels = Engine::UI::Panels;

Panels::PanelWindow::PanelWindow( const std::string& name, bool opened, const Settings::PanelWindowSettings& windowSettings)
    : m_opened { opened },
      m_windowSettings { m_windowSettings }
{
    m_autoSize = m_windowSettings.AutoSize;
}

void Panels::PanelWindow::Open()
{
    if (!m_opened)
    {
        m_opened = true;
        OpenEvent.Invoke();
    }
}

void Panels::PanelWindow::Close()
{
    if (m_opened)
    {
        m_opened = false;
        CloseEvent.Invoke();
    }
}

void Panels::PanelWindow::Focus()
{
    ImGui::SetWindowFocus((m_name + m_panelID).c_str());
}

void Panels::PanelWindow::SetOpened(bool value)
{
    if (value != m_opened)
    {
        m_opened = value;

        if (m_opened)
            OpenEvent.Invoke();
        else
            CloseEvent.Invoke();
    }
}

bool Panels::PanelWindow::IsOpened() const
{
    return m_opened;
}

bool Panels::PanelWindow::IsHovered() const
{
    return m_hovered;
}

bool Panels::PanelWindow::IsFocused() const
{
    return m_focused;
}

bool Panels::PanelWindow::IsAppearing() const
{
    if (ImGuiWindow* window = ImGui::FindWindowByName((m_name + GetPanelID()).c_str()); window)
        return window->Appearing;
    else
        return false;
}

void Panels::PanelWindow::ScrollToBottom()
{
    m_mustScrollToBottom = true;
}

void Panels::PanelWindow::ScrollToTop()
{
    m_mustScrollToTop = true;
}

bool Panels::PanelWindow::IsScrolledToBottom() const
{
    return m_scrolledToBottom;
}

bool Panels::PanelWindow::IsScrolledToTop() const
{
    return m_scrolledToTop;
}

void Panels::PanelWindow::DrawImpl()
{
    if (m_opened)
    {
        int windowFlags = ImGuiWindowFlags_None;

        if (!m_windowSettings.Resizable)               windowFlags |= ImGuiWindowFlags_NoResize;
        if (!m_windowSettings.Movable)                 windowFlags |= ImGuiWindowFlags_NoMove;
        if (!m_windowSettings.Dockable)                windowFlags |= ImGuiWindowFlags_NoDocking;
        if (m_windowSettings.HideBackground)           windowFlags |= ImGuiWindowFlags_NoBackground;
        if (m_windowSettings.ForceHorizontalScrollbar) windowFlags |= ImGuiWindowFlags_AlwaysHorizontalScrollbar;
        if (m_windowSettings.ForceVerticalScrollbar)   windowFlags |= ImGuiWindowFlags_AlwaysVerticalScrollbar;
        if (m_windowSettings.AllowHorizontalScrollbar) windowFlags |= ImGuiWindowFlags_HorizontalScrollbar;
        if (!m_windowSettings.BringToFrontOnFocus)     windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
        if (!m_windowSettings.Collapsable)             windowFlags |= ImGuiWindowFlags_NoCollapse;
        if (!m_windowSettings.AllowInputs)             windowFlags |= ImGuiWindowFlags_NoInputs;
        if (!m_windowSettings.Scrollable)              windowFlags |= ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar;
        if (!m_windowSettings.TitleBar)                windowFlags |= ImGuiWindowFlags_NoTitleBar;

        ImVec2 minSizeConstraint = Internal::Converter::ToImVec2(m_minSize);
        ImVec2 maxSizeConstraint = Internal::Converter::ToImVec2(m_maxSize);

        if (minSizeConstraint.x <= 0.0f || minSizeConstraint.y <= 0.0f)
            minSizeConstraint = { 0.0f, 0.0f };

        if (maxSizeConstraint.x <= 0.0f || maxSizeConstraint.y <= 0.0f)
            maxSizeConstraint = { 10000.0f, 10000.0f };

        ImGui::SetNextWindowSizeConstraints(minSizeConstraint, maxSizeConstraint);

        if (ImGui::Begin((m_name + m_panelID).c_str(), m_windowSettings.Closable ? &m_opened : nullptr, windowFlags))
        {
            m_hovered = ImGui::IsWindowHovered();
            m_focused = ImGui::IsWindowFocused();

            auto scrollY = ImGui::GetScrollY();

            m_scrolledToBottom = scrollY == ImGui::GetScrollMaxY();
            m_scrolledToTop = scrollY == 0.0f;

            if (!m_opened)
                CloseEvent.Invoke();

            Update();

            if (m_mustScrollToBottom)
            {
                ImGui::SetScrollY(ImGui::GetScrollMaxY());
                m_mustScrollToBottom = false;
            }

            if (m_mustScrollToTop)
            {
                ImGui::SetScrollY(0.0f);
                m_mustScrollToTop = false;
            }
        }

        ImGui::End();
    }
}
