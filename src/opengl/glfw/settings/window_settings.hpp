#pragma once
#ifndef ENGINE_GLFW_SETTINGS_WINDOW_SETTINGS_HPP_
#define ENGINE_GLFW_SETTINGS_WINDOW_SETTINGS_HPP_

#include <string>

namespace Engine
{
namespace GLFW
{
namespace Settings
{
    struct WindowSettings
    {
        static const int32_t DONT_CARE = -1;

        std::string Title = "OpenGL";

        uint16_t Width = 1280;
        uint16_t Height = 720;

        int16_t MinimumWidth  = DONT_CARE;
        int16_t MinimumHeight = DONT_CARE;
        int16_t MaximumWidth  = DONT_CARE;
        int16_t MaximumHeight = DONT_CARE;

        bool Fullscreen = false;
        bool Decorated  = true;
        bool Resizable  = false;
        bool Focused    = true;
        bool Maximized  = false;
        bool Visible    = true;

        int32_t RefreshRate = DONT_CARE;
        uint32_t Samples = 4;
    };
}
}
}

#endif // ENGINE_GLFW_SETTINGS_WINDOW_SETTINGS_HPP_
