#pragma once
#ifndef ENGINE_OPENGL_SETTINGS_RENDERING_CAPABILITY_HPP_
#define ENGINE_OPENGL_SETTINGS_RENDERING_CAPABILITY_HPP_

namespace Engine
{
namespace OpenGL
{
namespace Settings
{
    enum class ERenderingCapability
    {
        Blend                 = 0x0BE2,
        CullFace              = 0x0B44,
        DepthTest             = 0x0B71,
        Dither                = 0x0BD0,
        PolygonOffsetFill     = 0x8037,
        SampleAlphaToCoverage = 0x809E,
        SampleCoverage        = 0x80A0,
        ScissorTest           = 0x0C11,
        StencilTest           = 0x0B90,
        Multisample           = 0x809D 
    };
}
}
}

#endif // ENGINE_OPENGL_SETTINGS_RENDERING_CAPABILITY_HPP_
