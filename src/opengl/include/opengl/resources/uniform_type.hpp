#pragma once
#ifndef ENGINE_OPENGL_RESOURCES_UNIFORM_TYPE_HPP_
#define ENGINE_OPENGL_RESOURCES_UNIFORM_TYPE_HPP_

#include <stdint.h>

namespace Engine::OpenGL::Resources
{
    enum class UniformType : uint32_t
    {
        UniformBool        = 0x8B56,
        UniformInt         = 0x1404,
        UniformFloat       = 0x1406,
        UniformFloatVec2   = 0x8B50,
        UniformFloatVec3   = 0x8B51,
        UniformFloatVec4   = 0x8B52,
        UniformFloatMat4   = 0x8B5C,
        UniformDoubleMat4  = 0x8F48,
        UniformSampler2D   = 0x8B5E,
        UniformSamplerCube = 0x8B60
    };
}

#endif // ENGINE_OPENGL_RESOURCES_UNIFORM_TYPE_HPP_
