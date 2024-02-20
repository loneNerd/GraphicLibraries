#pragma once
#ifndef ENGINE_UI_SETTINGS_PANEL_WINDOW_SETTINGS_HPP_
#define ENGINE_UI_SETTINGS_PANEL_WINDOW_SETTINGS_HPP_

namespace Engine::UI::Settings
{
    struct PanelWindowSettings
    {
        bool Closable                 = false;
        bool Resizable                = true;
        bool Movable                  = true;
        bool Dockable                 = false;
        bool Scrollable               = true;
        bool HideBackground           = false;
        bool ForceHorizontalScrollbar = false;
        bool ForceVerticalScrollbar   = false;
        bool AllowHorizontalScrollbar = false;
        bool BringToFrontOnFocus      = true;
        bool Collapsable              = false;
        bool AllowInputs              = true;
        bool TitleBar                 = true;
        bool AutoSize                 = false;
    };
}

#endif // ENGINE_UI_SETTINGS_PANEL_WINDOW_SETTINGS_HPP_
