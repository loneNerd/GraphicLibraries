#pragma once
#ifndef GRAPHICLIBRARIES_OPEN_GL_OPENGL_HPP_
#define GRAPHICLIBRARIES_OPEN_GL_OPENGL_HPP_

#pragma comment(lib, "assimp.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")

#include <stdexcept>
#include <string>

#include <sdl2/SDL.h>

#include <gl/glew.h>

#include "engines/render_interface.hpp"
#include "2d_objects/triangle.hpp"

namespace GraphicLibraries
{

namespace Windows { class SDL2Window; class IWindow; }
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

        Windows::SDL2Window* m_window;
        Widgets::FpsCounter* m_fpsCounter;

        Objects2D::Triangle m_triangle;

        SDL_GLContext m_context;
    };
}
}

#endif // GRAPHICLIBRARIES_OPEN_GL_OPENGL_HPP_
