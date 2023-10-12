#pragma once
#ifndef GRAPHIC_LIBRARIES_OPENGL_OBJECTS_OPENGL_HPP_
#define GRAPHIC_LIBRARIES_OPENGL_OBJECTS_OPENGL_HPP_

#include <memory>
#include <stdexcept>
#include <string>

#include <GL/glew.h>

#include "objects/moveble_camera.hpp"
//#include "2d_objects/triangle.hpp"
#include "3d_models/cube.hpp"

namespace GraphicLibraries
{
namespace OpenGL
{

namespace Windows { class GLFWWindow; }
namespace Widgets { class FpsCounter; }

namespace Objects
{
    class OpenGL
    {
    public:
        OpenGL();
        ~OpenGL();

        OpenGL(const OpenGL& other)            = delete;
        OpenGL(OpenGL&& other)                 = delete;
        OpenGL& operator=(const OpenGL& other) = delete;
        OpenGL& operator=(OpenGL&& other)      = delete;

        void init();
        void release();
        void newFrame(float dt);

        std::shared_ptr<Windows::GLFWWindow> getWindow() const { return m_window; }

    private:
        const char* m_glslVersion = "#version 460";

        std::shared_ptr<Windows::GLFWWindow> m_window;
        Widgets::FpsCounter* m_fpsCounter;

        std::shared_ptr<MovebleCamera> m_camera;
        //Objects2D::Triangle m_triangle;
        Models3D::Cube m_cube;

        bool m_isInit = false;
    };
}
}
}

#endif // GRAPHIC_LIBRARIES_OPENGL_OBJECTS_OPENGL_HPP_
