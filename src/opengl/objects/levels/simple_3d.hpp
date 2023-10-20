#pragma once
#ifndef GRAPHIC_LIBRARIES_OPENGL_OBJECTS_LEVELS_SIMPLE_3D_HPP_
#define GRAPHIC_LIBRARIES_OPENGL_OBJECTS_LEVELS_SIMPLE_3D_HPP_

#include <memory>

#include "interfaces/level.hpp"

namespace GraphicLibraries
{
namespace OpenGL
{
namespace Objects
{

namespace Models3D { class Cube; }

namespace Levels
{
    class Simple3D : public Interfaces::ILevel
    {
    public:
        Simple3D();
        virtual ~Simple3D();

        Simple3D(const Simple3D& other)            = delete;
        Simple3D(Simple3D&& other)                 = delete;
        Simple3D& operator=(const Simple3D& other) = delete;
        Simple3D& operator=(Simple3D&& other)      = delete;

        void init(Windows::GLFWWindow* window) override;
        void release() override;
        void updateUI() override;
        void update(float dt) override;
        void draw() override;

    private:
        Windows::GLFWWindow* m_window = nullptr;
        std::unique_ptr<Models3D::Cube> m_cube = nullptr;
        bool m_isInit = false;
    };
}
}
}
}

#endif // GRAPHIC_LIBRARIES_OPENGL_OBJECTS_LEVELS_SIMPLE_3D_HPP_
