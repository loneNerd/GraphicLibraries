#pragma once
#ifndef ENGINE_GLFW_DEVICE_HPP_
#define ENGINE_GLFW_DEVICE_HPP_

#include "device_error.hpp"
#include "settings/device_settings.hpp"

namespace Engine
{
namespace GLFW
{
    class Device
    {
    public:
        Device(const Settings::DeviceSettings& deviceSettings);
        ~Device();

        Device(const Device& other)             = delete;
        Device(Device&& other)                  = delete;
        Device& operator=(const Device& other)  = delete;
        Device& operator=(const Device&& other) = delete;

        std::pair<int16_t, int16_t> GetMonitorSize() const;

        bool HasVsync() const;

        void SetVsync(bool value);
        void PollEvents() const;
        float GetElapsedTime() const;

    private:
        void bindErrorCallback();

        bool m_vsync = true;
        bool m_isAlive = false;
    };
}
}

#endif // ENGINE_GLFW_DEVICE_HPP_
