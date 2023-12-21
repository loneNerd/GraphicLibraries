#pragma once
#ifndef ENGINE_DEBUG_CONSOLE_COLOR_HPP_
#define ENGINE_DEBUG_CONSOLE_COLOR_HPP_

#include <iostream>
#include <windows.h>

#define COLOR_BLUE    Engine::Debug::Blue
#define COLOR_RED     Engine::Debug::Red
#define COLOR_GREEN   Engine::Debug::Green
#define COLOR_YELLOW  Engine::Debug::Yellow
#define COLOR_WHITE   Engine::Debug::White
#define COLOR_DEFAULT Engine::Debug::Grey

namespace Engine
{
namespace Debug
{
    inline std::ostream& Blue(std::ostream& s)
    {
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdout,
            FOREGROUND_BLUE | FOREGROUND_GREEN |
            FOREGROUND_INTENSITY);
        return s;
    }

    inline std::ostream& Red(std::ostream& s)
    {
        HANDLE hStdout = GetStdHandle(STD_ERROR_HANDLE);
        SetConsoleTextAttribute(hStdout,
            FOREGROUND_RED | FOREGROUND_INTENSITY);
        return s;
    }

    inline std::ostream& Green(std::ostream& s)
    {
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdout,
            FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        return s;
    }

    inline std::ostream& Yellow(std::ostream& s)
    {
        HANDLE hStdout = GetStdHandle(STD_ERROR_HANDLE);
        SetConsoleTextAttribute(hStdout,
            FOREGROUND_GREEN | FOREGROUND_RED |
            FOREGROUND_INTENSITY);
        return s;
    }

    inline std::ostream& White(std::ostream& s)
    {
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdout,
            FOREGROUND_RED | FOREGROUND_GREEN |
            FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        return s;
    }

    inline std::ostream& Grey(std::ostream& s)
    {
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdout,
            FOREGROUND_RED | FOREGROUND_GREEN |
            FOREGROUND_BLUE);
        return s;
    }

    struct color
    {
        color(WORD attribute) : m_color(attribute) { }

        WORD m_color;
    };

    template <class _Elem, class _Traits>
    std::basic_ostream<_Elem, _Traits>&
        operator<<(std::basic_ostream<_Elem, _Traits>& i, color& c)
    {
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdout, c.m_color);
        return i;
    }
}
}

#endif // ENGINE_DEBUG_CONSOLE_COLOR_HPP_
