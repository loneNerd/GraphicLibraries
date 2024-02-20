#pragma once
#ifndef ENGINE_UI_PANELS_BASE_PANEL_HPP_
#define ENGINE_UI_PANELS_BASE_PANEL_HPP_

#include <string>

#include "ui/drawable_interface.hpp"

namespace Engine::UI::Panels
{
    class BasePanel : public IDrawable
    {
    public:
        BasePanel();

        BasePanel(const BasePanel& other)             = delete;
        BasePanel(BasePanel&& other)                  = delete;
        BasePanel& operator=(const BasePanel& other)  = delete;
        BasePanel& operator=(const BasePanel&& other) = delete;

        void Draw() override;
        const std::string& GetPanelID() const;
        bool IsEnabled() const { return m_enabled; }

        void SetEnabled(bool enabled) { m_enabled = enabled; }

    protected:
        virtual void DrawImpl() = 0;

        std::string m_panelID = "";

    private:
        static uint64_t ID;
        bool m_enabled = true;
    };
}

#endif // ENGINE_UI_PANELS_BASE_PANEL_HPP_
