#pragma once
#ifndef ENGINE_WINDOWS_SETTINGS_WINDOW_SETTINGS_HPP_
#define ENGINE_WINDOWS_SETTINGS_WINDOW_SETTINGS_HPP_

#include <string>

#include "windows/settings/sdl2_window_flags.hpp"

namespace Engine::Windows::Settings
{
    struct WindowSettings
    {
        static const int32_t DONT_CARE = -1;

        std::string Title = "Engine";

        uint16_t Width = 1280;
        uint16_t Height = 720;

        int16_t MinimumWidth  = DONT_CARE;
        int16_t MinimumHeight = DONT_CARE;
        int16_t MaximumWidth  = DONT_CARE;
        int16_t MaximumHeight = DONT_CARE;

        ESDL2WindowFlags Flags = ESDL2WindowFlags::None;
        uint32_t Samples = 4;
    };
}

#endif // ENGINE_WINDOWS_SETTINGS_WINDOW_SETTINGS_HPP_
