#pragma once
#ifndef GRAPHICLIBRARIES_OPEN_GL_OPENGL_H_
#define GRAPHICLIBRARIES_OPEN_GL_OPENGL_H_

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")

#include <exception>
#include <string>

#include <sdl2/SDL.h>

#include <gl/glew.h>

#include "engines/render_interface.h"

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
        virtual void newFrame() override;

        virtual Windows::IWindow* getWindow() const override { return reinterpret_cast<Windows::IWindow*>(m_window); }

    private:
        const char* m_glslVersion = "#version 460";

        void checkError(int id, const char* message)
        {
            int success;
            char infoLog[512];
            glGetShaderiv(id, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(id, 512, nullptr, infoLog);
                throw std::exception(std::string("OPENGL: ").append(message).c_str());
            }
        }

        void loadShaders();
        void compileShader(const wchar_t* path, GLenum type);

        Windows::SDL2Window* m_window;
        Widgets::FpsCounter* m_fpsCounter;

        SDL_GLContext m_context;
        unsigned m_shaderProgram;
        unsigned m_vao;
        unsigned m_vbo;
    };
}
}

#endif // GRAPHICLIBRARIES_OPEN_GL_OPENGL_H_
