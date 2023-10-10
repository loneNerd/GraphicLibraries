#pragma once
#ifndef GRAPHIC_LIBRARIES_OPENGL_OBJECTS_SHADER_HPP_
#define GRAPHIC_LIBRARIES_OPENGL_OBJECTS_SHADER_HPP_

#include <GL/glew.h>

#include <glm/glm.hpp>

namespace GraphicLibraries
{
namespace OpenGL
{
namespace Objects
{
    class Shader
    {
    public:
        Shader() { }
        ~Shader() { if (m_isInit) release(); }

        Shader(const Shader& other)            = delete;
        Shader(Shader&& other)                 = delete;
        Shader& operator=(const Shader& other) = delete;
        Shader& operator=(Shader&& other)      = delete;

        void use() const { glUseProgram(m_id); }

        void load(const wchar_t* vertexPath, const wchar_t* fragmentPath, const wchar_t* geometryPath = nullptr);
        void release();

        void setFloat(const char* name, float value);
        void setInteger(const char* name, int value);
        void setVector2f(const char* name, float x, float y);
        void setVector2f(const char* name, const glm::vec2& value);
        void setVector3f(const char* name, float x, float y, float z);
        void setVector3f(const char* name, const glm::vec3& value);
        void setVector4f(const char* name, float x, float y, float z, float w);
        void setVector4f(const char* name, const glm::vec4& value);
        void setMatrix4(const char* name, const glm::mat4& matrix);

    private:
        void checkCompileErrors(unsigned id, const char* message);
        void compile(const wchar_t* path, GLenum type);

        unsigned m_id = 0;

        bool m_isInit = false;
    };
}
}
}

#endif // GRAPHIC_LIBRARIES_OPENGL_OBJECTS_SHADER_HPP_
