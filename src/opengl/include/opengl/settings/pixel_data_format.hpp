#pragma once
#ifndef ENGINE_OPENGL_SETTINGS_PIXEL_DATA_FORMAT_HPP_
#define ENGINE_OPENGL_SETTINGS_PIXEL_DATA_FORMAT_HPP_

namespace Engine::OpenGL::Settings
{
    enum class EPixelDataFormat
    {
        Color_Index     = 0x1900,
        Stencil_Index   = 0x1901,
        Depth_Component = 0x1902,
        Red             = 0x1903,
        Green           = 0x1904,
        Blue            = 0x1905,
        Alpha           = 0x1906,
        RGB             = 0x1907,
        BGR             = 0x80E0,
        RGBA            = 0x1908,
        BGRA            = 0x80E1,
        Luminance       = 0x1909,
        Luminance_Alpha = 0x190A,
    };
}

#endif // ENGINE_OPENGL_SETTINGS_PIXEL_DATA_FORMAT_HPP_
