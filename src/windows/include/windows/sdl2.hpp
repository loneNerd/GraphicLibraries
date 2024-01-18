#pragma once
#ifndef ENGINE_WINDOWS_SDL2_HPP_
#define ENGINE_WINDOWS_SDL2_HPP_

#include <memory>
#include <unordered_map>

#define SDL_MAIN_HANDLED
#include <sdl2/SDL.h>

#include "settings/window_settings.hpp"

namespace Engine
{
namespace Windows
{
    class SDL2
    {
    public:
        SDL2(const Settings::WindowSettings& windowSettings);
        ~SDL2();

        SDL2(const SDL2& other)             = delete;
        SDL2(SDL2&& other)                  = delete;
        SDL2& operator=(const SDL2& other)  = delete;
        SDL2& operator=(const SDL2&& other) = delete;

        void SetSize(uint16_t width, uint16_t height) { SDL_SetWindowSize(m_window, static_cast<int>(width), static_cast<int>(height)); }
        void SetPosition(int16_t x, int16_t y) { SDL_SetWindowPosition(m_window, static_cast<int>(x), static_cast<int>(y)); }
        void SetMinimumSize(int16_t minimumWidth, int16_t minimumHeight);
        void SetMaximumSize(int16_t maximumWidth, int16_t maximumHeight);
        void SetFullscreen(bool value);
        void SetTitle(const std::string& title);
        //void SetVsync(bool value);

        void Minimize() const { SDL_MinimizeWindow(m_window); }
        void Maximize() const { SDL_MaximizeWindow(m_window); }
        void Restore() const  { SDL_RestoreWindow(m_window); }
        void Hide() const     { SDL_HideWindow(m_window); }
        void Show() const     { SDL_ShowWindow(m_window); }
        void Focus() const    { SDL_SetWindowInputFocus(m_window); }

        bool ShouldClose() const { return !m_window; }

        void ToggleFullscreen() { SetFullscreen(!IsFullscreen()); }

        bool IsFullscreen() const { return SDL_GetWindowFlags(m_window) & SDL_WINDOW_FULLSCREEN; }
        bool IsHidden() const     { return SDL_GetWindowFlags(m_window) & SDL_WINDOW_HIDDEN; }
        bool IsVisible() const    { return SDL_GetWindowFlags(m_window) & SDL_WINDOW_SHOWN; }
        bool IsMaximized() const  { return SDL_GetWindowFlags(m_window) & SDL_WINDOW_MAXIMIZED; }
        bool IsMinimized() const  { return SDL_GetWindowFlags(m_window) & SDL_WINDOW_MINIMIZED; }
        bool IsFocused() const    { return SDL_GetWindowFlags(m_window) & SDL_WINDOW_INPUT_FOCUS; }
        bool IsResizable() const  { return SDL_GetWindowFlags(m_window) & SDL_WINDOW_RESIZABLE; }
        bool IsDecorated() const  { return SDL_GetWindowFlags(m_window) & SDL_WINDOW_BORDERLESS; }

        void SwapBuffers() const { SDL_GL_SwapWindow(m_window); }

        std::string GetTitle() const { return m_title; }
        //bool HasVsync() const { return m_vsync; }

        std::pair<int16_t, int16_t> GetMonitorSize() const;
        std::pair<uint16_t, uint16_t> GetSize() const;
        std::pair<int16_t, int16_t> GetMinimumSize() const { return m_minimumSize; }
        std::pair<int16_t, int16_t> GetMaximumSize() const { return m_maximumSize; }
        std::pair<int16_t, int16_t> GetPosition() const;

        SDL_Window* GetGlfwWindow() const { return m_window; }

        void PollEvents();

    private:
        void createGlfwWindow(const Settings::WindowSettings& windowSettings);
        void destroyWindow();

        // void bindKeyCallback() const;
        // void bindMouseCallback() const;
        // void bindResizeCallback() const;
        // void bindFramebufferResizeCallback() const;
        // void bindCursorMoveCallback() const;
        // void bindMoveCallback() const;
        // void bindFocusCallback() const;
        // void bindCloseCallback() const;

        void onResize(uint16_t width, uint16_t height);
        void onMove(int16_t x, int16_t y);

        void updateSizeLimit() const;

        SDL_Window* m_window = nullptr;
        SDL_GLContext m_context;

        std::string m_title = "";
        std::pair<uint16_t, uint16_t> m_size      = { 0, 0 };
        std::pair<int16_t, int16_t> m_minimumSize = { -1, -1 };
        std::pair<int16_t, int16_t> m_maximumSize = { -1, -1 };
        std::pair<int16_t, int16_t> m_position    = { -1, -1 };
    };
}
}

#endif // ENGINE_WINDOWS_SDL2_HPP_
