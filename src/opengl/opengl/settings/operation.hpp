#pragma once
#ifndef ENGINE_OPENGL_SETTINGS_OPERATION_HPP_
#define ENGINE_OPENGL_SETTINGS_OPERATION_HPP_

namespace Engine
{
namespace OpenGL
{
namespace Settings
{
    enum class EOperation
    {
        Keep          = 0x1E00,
        Zero          = 0,
        Replace       = 0x1E01,
        Increment     = 0x1E02,
        IncrementWrap = 0x8507,
        Decrement     = 0x1E03,
        DecrementWrap = 0x8508,
        Invert        = 0x150A
    };
}
}
}

#endif // ENGINE_OPENGL_SETTINGS_OPERATION_HPP_
