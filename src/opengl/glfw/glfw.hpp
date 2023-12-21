#pragma once
#ifndef ENGINE_GLFW_GLFW_HPP_
#define ENGINE_GLFW_GLFW_HPP_

#include <unordered_map>

#include <GLFW/glfw3.h>

#include "device.hpp"
#include "settings/window_settings.hpp"

namespace Engine
{
namespace GLFW
{
    class GLFW
    {
    public:
        GLFW(const Device& device, const Settings::WindowSettings& windowSettings);
        ~GLFW() { glfwDestroyWindow(m_glfwWindow); }

        GLFW(const GLFW& other)             = delete;
        GLFW(GLFW&& other)                  = delete;
        GLFW& operator=(const GLFW& other)  = delete;
        GLFW& operator=(const GLFW&& other) = delete;

        static GLFW* FindInstance(GLFWwindow* glfwWindow) { return WINDOWS_MAP.find(glfwWindow) != WINDOWS_MAP.end() ? WINDOWS_MAP[glfwWindow] : nullptr; }

        void SetSize(uint16_t width, uint16_t height) { glfwSetWindowSize(m_glfwWindow, static_cast<int>(width), static_cast<int>(height)); }
        void SetPosition(int16_t x, int16_t y) { glfwSetWindowPos(m_glfwWindow, static_cast<int>(x), static_cast<int>(y)); }
        void SetShouldClose(bool value) const { glfwSetWindowShouldClose(m_glfwWindow, value); }
        void SetCursorPosition(int16_t x, int16_t y) { glfwSetCursorPos(m_glfwWindow, static_cast<double>(x), static_cast<double>(y)); }
        void SetRefreshRate(int32_t refreshRate) { m_refreshRate = refreshRate; }
        void SetMinimumSize(int16_t minimumWidth, int16_t minimumHeight);
        void SetMaximumSize(int16_t maximumWidth, int16_t maximumHeight);
        void SetFullscreen(bool value);
        void SetTitle(const std::string& title);

        void Minimize() const { glfwIconifyWindow(m_glfwWindow); }
        void Maximize() const { glfwMaximizeWindow(m_glfwWindow); }
        void Restore() const  { glfwRestoreWindow(m_glfwWindow); }
        void Hide() const     { glfwHideWindow(m_glfwWindow); }
        void Show() const     { glfwShowWindow(m_glfwWindow); }
        void Focus() const    { glfwFocusWindow(m_glfwWindow); }

        bool ShouldClose() const { return glfwWindowShouldClose(m_glfwWindow); }

        void ToggleFullscreen() { SetFullscreen(!m_fullscreen); }

        bool IsFullscreen() const { return m_fullscreen; }
        bool IsHidden() const     { return glfwGetWindowAttrib(m_glfwWindow, GLFW_VISIBLE)   == GLFW_FALSE; }
        bool IsVisible() const    { return glfwGetWindowAttrib(m_glfwWindow, GLFW_VISIBLE)   == GLFW_TRUE; }
        bool IsMaximized() const  { return glfwGetWindowAttrib(m_glfwWindow, GLFW_MAXIMIZED) == GLFW_TRUE; }
        bool IsMinimized() const  { return glfwGetWindowAttrib(m_glfwWindow, GLFW_MAXIMIZED) == GLFW_FALSE; }
        bool IsFocused() const    { return glfwGetWindowAttrib(m_glfwWindow, GLFW_FOCUSED)   == GLFW_TRUE; }
        bool IsResizable() const  { return glfwGetWindowAttrib(m_glfwWindow, GLFW_RESIZABLE) == GLFW_TRUE; }
        bool IsDecorated() const  { return glfwGetWindowAttrib(m_glfwWindow, GLFW_DECORATED) == GLFW_TRUE; }

        void MakeCurrentContext() const { glfwMakeContextCurrent(m_glfwWindow); }
        void SwapBuffers() const        { glfwSwapBuffers(m_glfwWindow); }

        std::string GetTitle() const { return m_title; }

        std::pair<uint16_t, uint16_t> GetSize() const;
        std::pair<int16_t, int16_t> GetMinimumSize() const { return m_minimumSize; }
        std::pair<int16_t, int16_t> GetMaximumSize() const { return m_maximumSize; }
        std::pair<int16_t, int16_t> GetPosition() const;
        std::pair<uint16_t, uint16_t> GetFramebufferSize() const;

        int32_t GetRefreshRate() const { return m_refreshRate; }

        GLFWwindow* GetGlfwWindow() const { return m_glfwWindow; }

    private:
        void createGlfwWindow(const Settings::WindowSettings& windowSettings);

        void bindKeyCallback() const;
        void bindMouseCallback() const;
        void bindResizeCallback() const;
        void bindFramebufferResizeCallback() const;
        void bindCursorMoveCallback() const;
        void bindMoveCallback() const;
        void bindFocusCallback() const;
        void bindCloseCallback() const;

        void onResize(uint16_t width, uint16_t height);
        void onMove(int16_t x, int16_t y);

        void updateSizeLimit() const;

        static std::unordered_map<GLFWwindow*, GLFW*> WINDOWS_MAP;

        const Device& m_device;
        GLFWwindow* m_glfwWindow = nullptr;

        std::string m_title = "";
        std::pair<uint16_t, uint16_t> m_size      = { 0, 0 };
        std::pair<int16_t, int16_t> m_minimumSize = { -1, -1 };
        std::pair<int16_t, int16_t> m_maximumSize = { -1, -1 };
        std::pair<int16_t, int16_t> m_position    = { -1, -1 };
        bool m_fullscreen     = false;
        int32_t m_refreshRate = 0;
    };
}
}

#endif // ENGINE_WINDOW_WINDOW_HPP_
