#include "glfw.hpp"

#include <stdexcept>

namespace GLFW = Engine::GLFW;

std::unordered_map<GLFWwindow*, Engine::GLFW::GLFW*> GLFW::GLFW::WINDOWS_MAP;

GLFW::GLFW::GLFW(const Device& device, const Settings::WindowSettings& windowSettings)
    : m_device(device),
      m_title(windowSettings.Title),
      m_size { windowSettings.Width, windowSettings.Height },
      m_minimumSize { windowSettings.MinimumWidth, windowSettings.MinimumHeight },
      m_maximumSize { windowSettings.MaximumWidth, windowSettings.MaximumHeight },
      m_fullscreen(windowSettings.Fullscreen),
      m_refreshRate(windowSettings.RefreshRate)
{
    createGlfwWindow(windowSettings);

    bindKeyCallback();
    bindMouseCallback();
    bindCloseCallback();
    bindResizeCallback();
    bindCursorMoveCallback();
    bindFramebufferResizeCallback();
    bindMoveCallback();
    bindFocusCallback();
}

void GLFW::GLFW::SetMinimumSize(int16_t minimumWidth, int16_t minimumHeight)
{
    m_minimumSize.first = minimumWidth;
    m_minimumSize.second = minimumHeight;

    updateSizeLimit();
}

void GLFW::GLFW::SetMaximumSize(int16_t maximumWidth, int16_t maximumHeight)
{
    m_maximumSize.first = maximumWidth;
    m_maximumSize.second = maximumHeight;

    updateSizeLimit();
}

void GLFW::GLFW::SetFullscreen(bool value)
{
    if (value)
        m_fullscreen = true;

    glfwSetWindowMonitor
    (
        m_glfwWindow,
        value ? glfwGetPrimaryMonitor() : nullptr,
        static_cast<int>(m_position.first),
        static_cast<int>(m_position.second),
        static_cast<int>(m_size.first),
        static_cast<int>(m_size.second),
        m_refreshRate
    );

    if (!value)
        m_fullscreen = false;
}

void GLFW::GLFW::SetTitle(const std::string& title)
{
    m_title = title;
    glfwSetWindowTitle(m_glfwWindow, title.c_str());
}

std::pair<uint16_t, uint16_t> GLFW::GLFW::GetSize() const
{
    int width, height;
    glfwGetWindowSize(m_glfwWindow, &width, &height);
    return std::make_pair(static_cast<uint16_t>(width), static_cast<uint16_t>(height));
}

std::pair<int16_t, int16_t> GLFW::GLFW::GetPosition() const
{
    int x, y;
    glfwGetWindowPos(m_glfwWindow, &x, &y);
    return std::make_pair(static_cast<int16_t>(x), static_cast<int16_t>(y));
}

std::pair<uint16_t, uint16_t> GLFW::GLFW::GetFramebufferSize() const
{
    int width, height;
    glfwGetFramebufferSize(m_glfwWindow, &width, &height);
    return std::make_pair(static_cast<uint16_t>(width), static_cast<uint16_t>(height));
}

void GLFW::GLFW::createGlfwWindow(const Settings::WindowSettings& windowSettings)
{
    GLFWmonitor* selectedMonitor = nullptr;

    if (m_fullscreen)
        selectedMonitor = glfwGetPrimaryMonitor();

    glfwWindowHint(GLFW_RESIZABLE,    windowSettings.Resizable);
    glfwWindowHint(GLFW_DECORATED,    windowSettings.Decorated);
    glfwWindowHint(GLFW_FOCUSED,      windowSettings.Focused);
    glfwWindowHint(GLFW_MAXIMIZED,    windowSettings.Maximized);
    glfwWindowHint(GLFW_VISIBLE,      windowSettings.Visible);
    glfwWindowHint(GLFW_REFRESH_RATE, windowSettings.RefreshRate);
    glfwWindowHint(GLFW_SAMPLES,      windowSettings.Samples);

    m_glfwWindow = glfwCreateWindow(static_cast<int>(m_size.first), static_cast<int>(m_size.second), m_title.c_str(), selectedMonitor, nullptr);

    if (!m_glfwWindow)
    {
        throw std::runtime_error("Failed to create GLFW window");
    }
    else
    {
        updateSizeLimit();

        auto [x, y] = GetPosition();
        m_position.first = x;
        m_position.second = y;

        WINDOWS_MAP[m_glfwWindow] = this;
    }
}

void GLFW::GLFW::onResize(uint16_t width, uint16_t height)
{
    m_size.first = width;
    m_size.second = height;
}

void GLFW::GLFW::onMove(int16_t x, int16_t y)
{
    if (!m_fullscreen)
    {
        m_position.first  = x;
        m_position.second = y;
    }
}

void GLFW::GLFW::updateSizeLimit() const
{
    glfwSetWindowSizeLimits
    (
        m_glfwWindow,
        static_cast<int>(m_minimumSize.first),
        static_cast<int>(m_minimumSize.second),
        static_cast<int>(m_maximumSize.first),
        static_cast<int>(m_maximumSize.second)
    );
}

void GLFW::GLFW::bindKeyCallback() const
{
    auto keyCallback = [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        GLFW* windowInstance = FindInstance(window);

        if (windowInstance)
        {
        }
    };

    glfwSetKeyCallback(m_glfwWindow, keyCallback);
}

void GLFW::GLFW::bindMouseCallback() const
{
    auto mouseCallback = [](GLFWwindow* window, int button, int action, int mods)
    {
        GLFW* windowInstance = FindInstance(window);

        if (windowInstance)
        {
        }
    };

    glfwSetMouseButtonCallback(m_glfwWindow, mouseCallback);
}

void GLFW::GLFW::bindResizeCallback() const
{
    auto resizeCallback = [](GLFWwindow* window, int width, int height)
    {
        GLFW* windowInstance = FindInstance(window);

        if (windowInstance)
        {
        }
    };

    glfwSetWindowSizeCallback(m_glfwWindow, resizeCallback);
}

void GLFW::GLFW::bindFramebufferResizeCallback() const
{
    auto framebufferResizeCallback = [](GLFWwindow* window, int width, int height)
    {
        GLFW* windowInstance = FindInstance(window);

        if (windowInstance)
        {
        }
    };

    glfwSetFramebufferSizeCallback(m_glfwWindow, framebufferResizeCallback);
}

void GLFW::GLFW::bindCursorMoveCallback() const
{
    auto cursorMoveCallback = [](GLFWwindow* window, double x, double y)
    {
        GLFW* windowInstance = FindInstance(window);

        if (windowInstance)
        {
        }
    };

    glfwSetCursorPosCallback(m_glfwWindow, cursorMoveCallback);
}

void GLFW::GLFW::bindMoveCallback() const
{
    auto moveCallback = [](GLFWwindow* window, int x, int y)
    {
        GLFW* windowInstance = FindInstance(window);

        if (windowInstance)
        {
        }
    };

    glfwSetWindowPosCallback(m_glfwWindow, moveCallback);
}

void GLFW::GLFW::bindFocusCallback() const
{
    auto focusCallback = [](GLFWwindow* window, int focused)
    {
        GLFW* windowInstance = FindInstance(window);

        if (windowInstance)
        {
        }
    };

    glfwSetWindowFocusCallback(m_glfwWindow, focusCallback);
}

void GLFW::GLFW::bindCloseCallback() const
{
    auto closeCallback = [](GLFWwindow* window)
    {
        GLFW* windowInstance = FindInstance(window);

        if (windowInstance)
        {
        }
    };

    glfwSetWindowCloseCallback(m_glfwWindow, closeCallback);
}
