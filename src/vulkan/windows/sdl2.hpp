#pragma once
#ifndef GRAPHICLIBRARIES_WINDOWS_SDL2_H_
#define GRAPHICLIBRARIES_WINDOWS_SDL2_H_

#include <sdl2/SDL.h>

#include "windows_interface.hpp"

namespace GraphicLibraries
{
namespace Windows
{
    class SDL2Window : public IWindow
    {
    public:
        SDL2Window();
        virtual ~SDL2Window() override;

        SDL2Window(const SDL2Window& other)            = delete;
        SDL2Window(SDL2Window&& other)                 = delete;
        SDL2Window& operator=(const SDL2Window& other) = delete;
        SDL2Window& operator=(SDL2Window&& other)      = delete;

        virtual void release() override;

        virtual void initForOpenGL();
        virtual void initForVulkan();

        virtual void handleEvent() override;

        SDL_Window* getWindow()     const { return m_window; }
        const SDL_Event& getEvent() const { return m_event; }

        void setTitle(const char* title);

    protected:
        virtual void init() override;

    private:

        uint32_t m_flags = SDL_WINDOW_OPENGL;

        SDL_Window* m_window = nullptr;
        SDL_Event m_event;
    };
}
}

#endif // GRAPHICLIBRARIES_WINDOWS_SDL2_H_
