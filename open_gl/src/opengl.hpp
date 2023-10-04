#pragma once
#ifndef GRAPHICLIBRARIES_OPEN_GL_OPENGL_HPP_
#define GRAPHICLIBRARIES_OPEN_GL_OPENGL_HPP_

#pragma comment(lib, "assimp.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")

#include <stdexcept>
#include <string>

#include <gl/glew.h>

#include "camera.hpp"
#include "engines/render_interface.hpp"
//#include "2d_objects/triangle.hpp"
#include "3d_objects/cube.hpp"

namespace GraphicLibraries
{

namespace Windows { class GLFWWindow; class IWindow; }
namespace Widgets { class FpsCounter; }

namespace Engines
{
    class OpenGL : public IRender
    {
    public:
        OpenGL();
        virtual ~OpenGL() override;

        OpenGL(const OpenGL& other)            = delete;
        OpenGL(OpenGL&& other)                 = delete;
        OpenGL& operator=(const OpenGL& other) = delete;
        OpenGL& operator=(OpenGL&& other)      = delete;

        virtual void init() override;
        virtual void release() override;
        virtual void newFrame(float dt) override;

        virtual Windows::IWindow* getWindow() const override { return reinterpret_cast<Windows::IWindow*>(m_window); }

    private:
        const char* m_glslVersion = "#version 460";

        Windows::GLFWWindow* m_window;
        Widgets::FpsCounter* m_fpsCounter;

        Camera* m_camera;
        //Objects2D::Triangle m_triangle;
        Objects3D::Cube m_cube;
    };
}
}

#endif // GRAPHICLIBRARIES_OPEN_GL_OPENGL_HPP_
