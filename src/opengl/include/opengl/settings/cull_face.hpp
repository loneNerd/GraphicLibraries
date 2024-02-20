#pragma once
#ifndef ENGINE_OPENGL_SETTINGS_CULL_FACE_HPP_
#define ENGINE_OPENGL_SETTINGS_CULL_FACE_HPP_

namespace Engine::OpenGL::Settings
{
    enum class ECullFace
    {
        Front        = 0x0404,
        Back         = 0x0405,
        FrontAndBack = 0x0408
    };
}

#endif // ENGINE_OPENGL_SETTINGS_CULL_FACE_HPP_
