#include "window_sdl2.h"

using namespace GraphicLibraries::UI;

WindowSDL2::WindowSDL2()
    : m_event { SDL_FIRSTEVENT }
{
    std::cout << "Initializing SDL2 Window... ";

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER))
        throw std::exception(SDL_GetError());

    m_window = SDL_CreateWindow("",
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                INITIAL_WIDTH,
                                INITIAL_HEIGHT,
                                0);

    m_renderer = SDL_CreateRenderer(m_window, -1,
                                    SDL_RENDERER_ACCELERATED);

    std::cout << "Done" << std::endl;
}

WindowSDL2::~WindowSDL2()
{
    std::cout << "Releasing SDL2 Window...";

    if (m_renderer)
        SDL_DestroyRenderer(m_renderer);

    if (m_window)
        SDL_DestroyWindow(m_window);

    SDL_Quit();

    std::cout << " Done";
}

bool WindowSDL2::render()
{
    SDL_PollEvent(&m_event);
    switch (m_event.type)
    {
        case SDL_QUIT:
        {
            return false;
        }
        default:
        {
            return true;
        }
    }
    return true;
}


HWND WindowSDL2::getHWND() const
{
    SDL_SysWMinfo sysWMInfo;
    SDL_VERSION(&sysWMInfo.version);
    SDL_GetWindowWMInfo(m_window, &sysWMInfo);
    return sysWMInfo.info.win.window;
}

int WindowSDL2::getWidth() const
{
    int width;
    SDL_GetWindowSize(m_window, &width, nullptr);
    return width;
}

int WindowSDL2::getHeight() const
{
    int height;
    SDL_GetWindowSize(m_window, nullptr, &height);
    return height;
}

void WindowSDL2::setTitle(const char* title)
{
    SDL_SetWindowTitle(m_window, title);
}
