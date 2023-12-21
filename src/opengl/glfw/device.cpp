#include "device.hpp"

#include <stdexcept>

#include <GLFW/glfw3.h>

namespace GLFW = Engine::GLFW;

GLFW::Device::Device(const Settings::DeviceSettings& deviceSettings)
{
    bindErrorCallback();

    int initializationCode = glfwInit();

    if (initializationCode == GLFW_FALSE)
    {
        throw std::runtime_error("Failed to Init GLFW");
        glfwTerminate();
    }
    else
    {
        if (deviceSettings.DebugProfile)
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, deviceSettings.ContextMajorVersion);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, deviceSettings.ContextMinorVersion);

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, deviceSettings.Samples);

        m_isAlive = true;
    }
}

GLFW::Device::~Device()
{
    if (m_isAlive)
    {
        glfwTerminate();
    }
}

std::pair<int16_t, int16_t> GLFW::Device::GetMonitorSize() const
{
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    return std::pair<int16_t, int16_t>(static_cast<int16_t>(mode->width), static_cast<int16_t>(mode->height));
}

bool GLFW::Device::HasVsync() const
{
    return m_vsync;
}

void GLFW::Device::SetVsync(bool value)
{
    glfwSwapInterval(value ? 1 : 0);
    m_vsync = value;
}

void GLFW::Device::PollEvents() const
{
    glfwPollEvents();
}

float GLFW::Device::GetElapsedTime() const
{
    return static_cast<float>(glfwGetTime());
}

void GLFW::Device::bindErrorCallback()
{
    auto errorCallback = [](int code, const char* description)
    {
        throw std::runtime_error(description);
    };

    glfwSetErrorCallback(errorCallback);
}
