#pragma once
#ifndef GRAPHICLIBRARIES_WINDOWS_WIN32_H_
#define GRAPHICLIBRARIES_WINDOWS_WIN32_H_

#include <Windows.h>
#include <WinUser.h>

#include "windows_interface.hpp"

namespace GraphicLibraries
{
namespace Windows
{
    class Win32Window : public IWindow
    {
    public:
        Win32Window();
        virtual ~Win32Window() override;

        Win32Window(const Win32Window& other)            = delete;
        Win32Window(Win32Window&& other)                 = delete;
        Win32Window& operator=(const Win32Window& other) = delete;
        Win32Window& operator=(Win32Window&& other)      = delete;

        virtual void init()        override;
        virtual void release()     override;
        virtual void handleEvent() override;

        void setTitle(const wchar_t* title);

        void showWindow();

        HWND getHwnd() const { return m_hwnd; }

    private:
        static LRESULT WINAPI s_processes(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        LRESULT WINAPI processes(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

        WNDCLASSEX m_windowClass;
        HWND m_hwnd;
    };
}
}

#endif //GRAPHICLIBRARIES_WINDOWS_WIN32_H_
