#pragma once
#ifndef ENGINE_GLFW_SETTINGS_DEVICE_SETTINGS_HPP_
#define ENGINE_GLFW_SETTINGS_DEVICE_SETTINGS_HPP_

#include <string>

namespace Engine
{
namespace GLFW
{
namespace Settings
{
    struct DeviceSettings
    {
        bool DebugProfile         = false;
        bool ForwardCompatibility = false;

        uint8_t ContextMajorVersion = 4;
        uint8_t ContextMinorVersion = 6;
        uint8_t Samples             = 4;
    };
}
}
}

#endif // ENGINE_GLFW_SETTINGS_DEVICE_SETTINGS_HPP_
