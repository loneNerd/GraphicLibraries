#pragma once
#ifndef GRAPHICLIBRARIES_UI_WINDOW_SDL2_H_
#define GRAPHICLIBRARIES_UI_WINDOW_SDL2_H_

#include <iostream>
#include <exception>
#include <windows.h>

#include <SDL.h>
#include <SDL_syswm.h>

//#include "engines/directx_11.h"

namespace GraphicLibraries
{
namespace UI
{
    class WindowSDL2
    {
    public:
        WindowSDL2();
        ~WindowSDL2();

        WindowSDL2(const WindowSDL2& other)            = delete;
        WindowSDL2(WindowSDL2&& other)                 = delete;
        WindowSDL2& operator=(const WindowSDL2& other) = delete;
        WindowSDL2& operator=(WindowSDL2&& other)      = delete;

        HWND getHWND() const;
        int getWidth() const;
        int getHeight() const;

        bool render();

        void setTitle(const char* title);

    private:
        const int INITIAL_WIDTH  = 1280;
        const int INITIAL_HEIGHT = 720;

        SDL_Window*   m_window   = nullptr;
        SDL_Renderer* m_renderer = nullptr;
        SDL_Event     m_event;
    };
}
}

#endif // GRAPHICLIBRARIES_UI_WINDOW_SDL2_H_
