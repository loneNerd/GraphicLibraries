#pragma once
#ifndef ENGINE_LOGS_CONSOLE_COLOR_HPP_
#define ENGINE_LOGS_CONSOLE_COLOR_HPP_

#include <iostream>
#include <windows.h>

#define COLOR_BLUE    Engine::Logs::Blue
#define COLOR_RED     Engine::Logs::Red
#define COLOR_GREEN   Engine::Logs::Green
#define COLOR_YELLOW  Engine::Logs::Yellow
#define COLOR_WHITE   Engine::Logs::White
#define COLOR_DEFAULT Engine::Logs::Grey

namespace Engine::Logs
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

#endif // ENGINE_LOGS_CONSOLE_COLOR_HPP_
