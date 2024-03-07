#include "windows/sdl2.hpp"

#include <stdexcept>

#include "logs/logger.hpp"

namespace Windows = Engine::Windows;

Windows::SDL2::SDL2(const Settings::WindowSettings& windowSettings)
    : m_title(windowSettings.Title),
      m_size { windowSettings.Width, windowSettings.Height },
      m_minimumSize { windowSettings.MinimumWidth, windowSettings.MinimumHeight },
      m_maximumSize { windowSettings.MaximumWidth, windowSettings.MaximumHeight }
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER))
    {
        throw std::runtime_error(SDL_GetError());
        SDL_Quit();
    }

    createSDL2Window(windowSettings);

    ResizeEvent += std::bind(&SDL2::onResize, this, std::placeholders::_1, std::placeholders::_2);
    MoveEvent += std::bind(&SDL2::onMove, this, std::placeholders::_1, std::placeholders::_2);
}

Windows::SDL2::~SDL2()
{
    ResizeEvent.RemoveAllListeners();

    SDL_Quit();
}

void Windows::SDL2::CloseWindow()
{
    destroyWindow();
}

void Windows::SDL2::PollEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_KEYDOWN:
            {
                KeyPressedEvent.Invoke(event.key.keysym.sym);
                break;
            }
            case SDL_KEYUP:
            {
                KeyReleasedEvent.Invoke(event.key.keysym.sym);
                break;
            }
            case SDL_MOUSEBUTTONDOWN:
            {
                MouseButtonPressedEvent.Invoke(event.button.button);
                break;
            }
            case SDL_MOUSEBUTTONUP:
            {
                MouseButtonReleasedEvent.Invoke(event.button.button);
                break;
            }
            case SDL_MOUSEMOTION:
            {
                CursorMoveEvent.Invoke(event.motion.x, event.motion.y);
                break;
            }
            case SDL_WINDOWEVENT:
            {
                switch (event.window.event)
                {
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                    {
                        ResizeEvent.Invoke(event.window.data1, event.window.data2);
                        break;
                    }
                    case SDL_WINDOWEVENT_MOVED:
                    {
                        MoveEvent.Invoke(event.window.data1, event.window.data2);
                        break;
                    }
                    case SDL_WINDOWEVENT_MINIMIZED:
                    {
                        MinimizeEvent.Invoke();
                        break;
                    }
                    case SDL_WINDOWEVENT_RESTORED:
                    {
                        RestoredEvent.Invoke();
                        break;
                    }
                    case SDL_WINDOWEVENT_MAXIMIZED:
                    {
                        MaximizeEvent.Invoke();
                        break;
                    }
                    case SDL_WINDOWEVENT_FOCUS_GAINED:
                    {
                        GainFocusEvent.Invoke();
                        break;
                    }
                    case SDL_WINDOWEVENT_FOCUS_LOST:
                    {
                        LostFocusEvent.Invoke();
                        break;
                    }
                    case SDL_WINDOWEVENT_CLOSE:
                    {
                        CloseEvent.Invoke();
                        destroyWindow();
                        break;
                    }
                }
                break;
            }
            case SDL_QUIT:
            {
                destroyWindow();
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

void Windows::SDL2::SetMinimumSize(int16_t minimumWidth, int16_t minimumHeight)
{
    m_minimumSize.first = minimumWidth;
    m_minimumSize.second = minimumHeight;

    updateSizeLimit();
}

void Windows::SDL2::SetMaximumSize(int16_t maximumWidth, int16_t maximumHeight)
{
    m_maximumSize.first = maximumWidth;
    m_maximumSize.second = maximumHeight;

    updateSizeLimit();
}

void Windows::SDL2::SetFullscreen(bool value)
{
    SDL_SetWindowFullscreen(m_window, value ? 0 : SDL_WINDOW_FULLSCREEN);
}

void Windows::SDL2::SetTitle(const std::string& title)
{
    m_title = title;
    SDL_SetWindowTitle(m_window, title.c_str());
}

std::pair<int16_t, int16_t> Windows::SDL2::GetMonitorSize() const
{
    SDL_DisplayMode displayMode;

    if (SDL_GetDesktopDisplayMode(0, &displayMode) != 0)
    {
        std::string message = "SDL_GetDesktopDisplayMode failed: ";
        std::string glewError = SDL_GetError();
        LOG_ERROR(message + glewError);
        return std::pair<int16_t, int16_t>(-1, -1);
    }

    return std::pair<int16_t, int16_t>(static_cast<int16_t>(displayMode.w), static_cast<int16_t>(displayMode.h));
}

std::pair<uint16_t, uint16_t> Windows::SDL2::GetSize() const
{
    int width = -1;
    int height = -1;
    SDL_GetWindowSize(m_window, &width, &height);
    return std::make_pair(static_cast<uint16_t>(width), static_cast<uint16_t>(height));
}

std::pair<int16_t, int16_t> Windows::SDL2::GetPosition() const
{
    int x = -1;
    int y = -1;
    SDL_GetWindowPosition(m_window, &x, &y);
    return std::make_pair(static_cast<int16_t>(x), static_cast<int16_t>(y));
}

void Windows::SDL2::createSDL2Window(const Settings::WindowSettings& windowSettings)
{
    m_window = SDL_CreateWindow("",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        static_cast<int>(m_size.first),
        static_cast<int>(m_size.second),
        static_cast<Uint32>(windowSettings.Flags));

    if (!m_window)
    {
        throw std::runtime_error("Failed to create SDL2 window");
    }
    else
    {
        updateSizeLimit();

        auto [x, y] = GetPosition();
        m_position.first = x;
        m_position.second = y;

        m_context = SDL_GL_CreateContext(m_window);

        if (m_context == NULL)
            throw std::exception("OPENGL: Can't create SDL context");

        SDL_GL_MakeCurrent(m_window, m_context);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, windowSettings.Samples);
    }
}

void Windows::SDL2::destroyWindow()
{
    if (m_window)
    {
        SDL_GL_DeleteContext(m_context);
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
}

void Windows::SDL2::onResize(uint16_t width, uint16_t height)
{
    m_size.first = width;
    m_size.second = height;
}

void Windows::SDL2::onMove(int16_t x, int16_t y)
{
    if (!IsFullscreen())
    {
        m_position.first  = x;
        m_position.second = y;
    }
}

void Windows::SDL2::updateSizeLimit() const
{
    SDL_SetWindowMinimumSize(m_window, static_cast<int>(m_minimumSize.first), static_cast<int>(m_minimumSize.second));
    SDL_SetWindowMaximumSize(m_window, static_cast<int>(m_maximumSize.first), static_cast<int>(m_maximumSize.second));
}
