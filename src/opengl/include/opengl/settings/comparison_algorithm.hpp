#pragma once
#ifndef ENGINE_OPENGL_SETTINGS_COMPARISON_ALGORITHM_HPP_
#define ENGINE_OPENGL_SETTINGS_COMPARISON_ALGORITHM_HPP_

namespace Engine::OpenGL::Settings
{
    enum class EComparaisonAlgorithm
    {
        Never        = 0x0200,
        Less         = 0x0201,
        Equal        = 0x0202,
        LessEqual    = 0x0203,
        Greater      = 0x0204,
        Notequal     = 0x0205,
        GreaterEqual = 0x0206,
        Always       = 0x0207
    };
}

#endif // ENGINE_OPENGL_SETTINGS_COMPARISON_ALGORITHM_HPP_
