#include "win32.hpp"

#include <iostream>

#include <imgui/backends/imgui_impl_win32.h>

using namespace GraphicLibraries::Windows;

Win32Window::Win32Window()
    : m_windowClass { NULL },
      m_hwnd { nullptr }
{
    m_isClosed = true;
    m_isInit = false;
}

Win32Window::~Win32Window()
{
    std::cout << "Win32 Window Destructor" << std::endl;

    if (m_isInit)
        release();
}

void Win32Window::init()
{
    std::cout << "Initializing Win32 Window" << std::endl;

    m_windowClass =
    {
        sizeof(WNDCLASSEX),
        CS_CLASSDC,
        s_processes,
        0L, 0L,
        GetModuleHandle(nullptr),
        nullptr, nullptr, nullptr, nullptr,
        "main_window",
        nullptr
    };

    if (!::RegisterClassEx(&m_windowClass))
        throw std::exception("WIN 32: Can't register window class");

    m_hwnd = ::CreateWindow(m_windowClass.lpszClassName,
                            "",
                            WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT, CW_USEDEFAULT,
                            INITIAL_WIDTH, INITIAL_HEIGHT,
                            nullptr, nullptr,
                            m_windowClass.hInstance,
                            this);

    if (!m_hwnd)
        throw std::exception("WIN32: Can't create HWND");

    m_isInit = true;
    m_isClosed = false;
}

void Win32Window::release()
{
    std::cout << "Releasing Win32 Window" << std::endl;

    ::DestroyWindow(m_hwnd);
    ::UnregisterClass(m_windowClass.lpszClassName, m_windowClass.hInstance);

    m_isInit = false;
}

void Win32Window::showWindow()
{
    // Show the window
    ::ShowWindow(m_hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(m_hwnd);
}

void Win32Window::handleEvent()
{
    MSG msg;

    while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }
}

LRESULT WINAPI Win32Window::s_processes(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Win32Window* pThis;

    if (uMsg == WM_NCCREATE)
    {
        pThis = static_cast<Win32Window*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);

        SetLastError(0);
        if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis)))
        {
            if (GetLastError() != 0)
                return FALSE;
        }
    }
    else
        pThis = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    if (pThis)
        return pThis->processes(hWnd, uMsg, wParam, lParam);

    return FALSE;
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI Win32Window::processes(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true; // ::DefWindowProc(hWnd, msg, wParam, lParam);

    switch (msg)
    {
        case WM_SYSCOMMAND:
        {
            if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
                return 0;
            break;
        }
        case WM_KEYDOWN:
        {
            if (wParam == VK_ESCAPE)
                m_isClosed = true;
            break;
        }
        case WM_DESTROY:
        {
            m_isClosed = true;
            ::PostQuitMessage(0);
            return 0;
        }
    }

    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

void Win32Window::setTitle(const char* title)
{
    SetWindowText(m_hwnd, title);
}
