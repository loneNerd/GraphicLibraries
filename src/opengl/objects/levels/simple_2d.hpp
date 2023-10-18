#pragma once
#ifndef GRAPHIC_LIBRARIES_OPENGL_OBJECTS_LEVELS_SIMPLE_2D_HPP_
#define GRAPHIC_LIBRARIES_OPENGL_OBJECTS_LEVELS_SIMPLE_2D_HPP_

#include <memory>

#include "interfaces/level.hpp"

namespace GraphicLibraries
{
namespace OpenGL
{
namespace Objects
{

namespace Models2D { class Triangle; }

namespace Levels
{
    class Simple2D : public Interfaces::ILevel
    {
    public:
        Simple2D();
        virtual ~Simple2D();

        Simple2D(const Simple2D& other)            = delete;
        Simple2D(Simple2D&& other)                 = delete;
        Simple2D& operator=(const Simple2D& other) = delete;
        Simple2D& operator=(Simple2D&& other)      = delete;

        void init(Windows::GLFWWindow* window) override;
        void release() override;
        void update(float dt) override;
        void draw() override;

    private:
        Windows::GLFWWindow* m_window = nullptr;
        std::unique_ptr<Models2D::Triangle> m_triangle = nullptr;
        bool m_isInit = false;
    };
}
}
}
}

#endif // GRAPHIC_LIBRARIES_OPENGL_OBJECTS_LEVELS_SIMPLE_2D_HPP_
