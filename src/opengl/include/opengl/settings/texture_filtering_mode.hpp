#pragma once
#ifndef ENGINE_OPENGL_SETTINGS_TEXTURE_FILTERIG_MODE_HPP_
#define ENGINE_OPENGL_SETTINGS_TEXTURE_FILTERIG_MODE_HPP_

namespace Engine::OpenGL::Settings
{
    enum class ETextureFilteringMode
    {
        Nearest               = 0x2600,
        Linear                = 0x2601,
        NearestMipmap_Nearest = 0x2700,
        LinearMipmapLinear    = 0x2703,
        LinearMipmapNearest   = 0x2701,
        NearestMipmapLinear   = 0x2702
    };
}

#endif // ENGINE_OPENGL_SETTINGS_TEXTURE_FILTERIG_MODE_HPP_
