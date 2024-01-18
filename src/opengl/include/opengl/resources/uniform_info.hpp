#pragma once
#ifndef ENGINE_OPENGL_RESOURCES_UNIFORM_INFO_HPP_
#define ENGINE_OPENGL_RESOURCES_UNIFORM_INFO_HPP_

#include <string>
#include <any>

#include "uniform_type.hpp"

namespace Engine
{
namespace OpenGL
{
namespace Resources
{
    struct UniformInfo
    {
        UniformType Type;
        std::string Name;
        uint32_t    Location;
        std::any    DefaultValue;
    };
}
}
}

#endif // ENGINE_OPENGL_RESOURCES_UNIFORM_INFO_HPP_
