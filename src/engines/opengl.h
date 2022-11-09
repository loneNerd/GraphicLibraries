#pragma once
#ifndef GRAPHICLIBRARIES_ENGINES_OPENGL_H_
#define GRAPHICLIBRARIES_ENGINES_OPENGL_H_

#include <string>
#include <sstream>

#include <gl/glew.h>

#include "utils.h"
#include "ui/window_sdl2.h"

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")

namespace GraphicLibraries
{
namespace Engines
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

        void render();

    private:
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

        void compileShaders();

        SDL_GLContext m_context;
        unsigned m_shaderProgram;
        unsigned m_vao;
        unsigned m_vbo;
    };
}
}

#endif // GRAPHICLIBRARIES_ENGINES_OPENGL_H_
