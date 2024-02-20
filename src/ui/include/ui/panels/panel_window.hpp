#pragma once
#ifndef ENGINE_UI_PANELS_PANEL_WINDOW_HPP_
#define ENGINE_UI_PANELS_PANEL_WINDOW_HPP_

#include "ui/panels/panel_transformable.hpp"
#include "ui/settings/panel_window_settings.hpp"
#include "tools/eventing/event.hpp"

namespace Engine::UI::Panels
{
    class PanelWindow : public APanelTransformable
    {
    public:
        PanelWindow
        (
            const std::string& name = "",
            bool opened = true,
            const Settings::PanelWindowSettings& windowSettings = Settings::PanelWindowSettings { }
        );

        PanelWindow(const PanelWindow& other)             = delete;
        PanelWindow(PanelWindow&& other)                  = delete;
        PanelWindow& operator=(const PanelWindow& other)  = delete;
        PanelWindow& operator=(const PanelWindow&& other) = delete;

        void Open();
        void Close();
        void Focus();
        void SetOpened(bool value);
        bool IsOpened() const;
        bool IsHovered() const;
        bool IsFocused() const;
        bool IsAppearing() const;
        void ScrollToBottom();
        void ScrollToTop();
        bool IsScrolledToBottom() const;
        bool IsScrolledToTop() const;

        Tools::Eventing::Event<> OpenEvent;
        Tools::Eventing::Event<> CloseEvent;

    protected:
        void DrawImpl() override;

        std::string m_name = "";

        Math::FVector2 m_minSize = { 0.0f, 0.0f };
        Math::FVector2 m_maxSize = { 0.0f, 0.0f };

        Settings::PanelWindowSettings m_windowSettings;

    private:
        bool m_opened             = true;
        bool m_hovered            = true;
        bool m_focused            = true;
        bool m_mustScrollToBottom = false;
        bool m_mustScrollToTop    = false;
        bool m_scrolledToBottom   = false;
        bool m_scrolledToTop      = false;
    };
}

#endif // ENGINE_UI_PANELS_PANEL_WINDOW_HPP_
