#include "sdl2.h"

#include <iostream>

using namespace GraphicLibraries::Windows;

SDL2Window::SDL2Window()
    : m_window { nullptr },
      m_event { SDL_FIRSTEVENT }
{
    m_isClosed = true;
    m_isInit = false;
}

SDL2Window::~SDL2Window()
{
    std::cout << "SDL2Window Destructor" << std::endl;

    if (m_isInit)
        release();
}

void SDL2Window::initForOpenGL()
{
    m_flags = SDL_WINDOW_OPENGL;
    init();
}

void SDL2Window::initForVulkan()
{
    m_flags = SDL_WINDOW_VULKAN;
    init();
}

void SDL2Window::init()
{
    std::cout << "Initializing SDL2 Window" << std::endl;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER))
        throw std::exception(SDL_GetError());

    m_window = SDL_CreateWindow("",
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                INITIAL_WIDTH,
                                INITIAL_HEIGHT,
                                SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | m_flags);

    if (!m_window)
        throw std::exception(SDL_GetError());

    m_isInit = true;
    m_isClosed = false;
}

void SDL2Window::release()
{
    std::cout << "Releasing SDL2 Window" << std::endl;

    if (m_window)
        SDL_DestroyWindow(m_window);

    SDL_Quit();

    m_isInit = false;
}

void SDL2Window::handleEvent()
{
    SDL_PollEvent(&m_event);

    switch (m_event.type)
    {
        case SDL_KEYDOWN:
        {
            if (m_event.key.keysym.sym == SDLK_ESCAPE)
                m_isClosed = true;
            break;
        }
        case SDL_WINDOWEVENT:
        {
            if (m_event.window.event == SDL_WINDOWEVENT_CLOSE)
                m_isClosed = true;
            break;
        }
        case SDL_QUIT:
        {
            m_isClosed = true;
            break;
        }
        default:
        {
            break;
        }
    }
}

void SDL2Window::setTitle(const char* title)
{
    SDL_SetWindowTitle(m_window, title);
}
