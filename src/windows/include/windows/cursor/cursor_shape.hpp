#pragma once
#ifndef ENGINE_WINDOWS_CURSOR_CURSOR_SHAPE_HPP_
#define ENGINE_WINDOWS_CURSOR_CURSOR_SHAPE_HPP_

#include <stdint.h>

namespace Engine::Windows::Cursor
{
    enum class ECursorShape
    {
        Arrow,
        IBeam,
        Wait,
        Crosshair,
        WaitArrow,
        SizeNWSE,
        SizeNESW,
        SizeWE,
        SizeNS,
        SizeAll,
        No,
        Hand,
        System
    };
}

#endif // ENGINE_WINDOWS_CURSOR_CURSOR_SHAPE_HPP_
