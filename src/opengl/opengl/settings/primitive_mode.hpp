#pragma once
#ifndef ENGINE_OPENGL_SETTINGS_PRIMITIVE_MODE_HPP_
#define ENGINE_OPENGL_SETTINGS_PRIMITIVE_MODE_HPP_

namespace Engine
{
namespace OpenGL
{
namespace Settings
{
    enum class EPrimitiveMode
    {
        Points                 = 0x0000,
        Lines                  = 0x0001,
        LineLoop               = 0x0002,
        LineStrip              = 0x0003,
        Triangles              = 0x0004,
        TriangleStrip          = 0x0005,
        TriangleFan            = 0x0006,
        LinesAdjacency         = 0x000A,
        LineStripAdjacency     = 0x000B,
        TrianglesAdjacency     = 0x000C,
        TriangleStripAdjacency = 0x000D,
        Patches                = 0xE,
    };
}
}
}

#endif // ENGINE_OPENGL_SETTINGS_PRIMITIVE_MODE_HPP_
