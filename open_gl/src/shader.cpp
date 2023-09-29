#include "shader.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include <glm/gtc/type_ptr.hpp>

#include "utils.hpp"

using namespace GraphicLibraries::Engines;

void Shader::load(const wchar_t* vertexPath, const wchar_t* fragmentPath, const wchar_t* geometryPath)
{
    m_id = glCreateProgram();

    compile(vertexPath, GL_VERTEX_SHADER);
    compile(fragmentPath, GL_FRAGMENT_SHADER);

    if (geometryPath) compile(geometryPath, GL_GEOMETRY_SHADER);

    glLinkProgram(m_id);

    m_isInit = true;
}

void Shader::release()
{
    glDeleteProgram(m_id);

    m_isInit = false;
}

void Shader::setFloat(const char* name, float value)
{
    glUseProgram(m_id);
    glUniform1f(glGetUniformLocation(m_id, name), value);
}

void Shader::setInteger(const char* name, int value)
{
    glUseProgram(m_id);
    glUniform1i(glGetUniformLocation(m_id, name), value);
}

void Shader::setVector2f(const char* name, float x, float y)
{
    glUseProgram(m_id);
    glUniform2f(glGetUniformLocation(m_id, name), x, y);
}

void Shader::setVector2f(const char* name, const glm::vec2& value)
{
    glUseProgram(m_id);
    glUniform2f(glGetUniformLocation(m_id, name), value.x, value.y);
}

void Shader::setVector3f(const char* name, float x, float y, float z)
{
    glUseProgram(m_id);
    glUniform3f(glGetUniformLocation(m_id, name), x, y, z);
}

void Shader::setVector3f(const char* name, const glm::vec3& value)
{
    glUseProgram(m_id);
    glUniform3f(glGetUniformLocation(m_id, name), value.x, value.y, value.z);
}

void Shader::setVector4f(const char* name, float x, float y, float z, float w)
{
    glUseProgram(m_id);
    glUniform4f(glGetUniformLocation(m_id, name), x, y, z, w);
}

void Shader::setVector4f(const char* name, const glm::vec4& value)
{
    glUseProgram(m_id);
    glUniform4f(glGetUniformLocation(m_id, name), value.x, value.y, value.z, value.w);
}

void Shader::setMatrix4(const char* name, const glm::mat4& matrix)
{
    glUseProgram(m_id);
    glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, false, glm::value_ptr(matrix));
}

void Shader::checkCompileErrors(unsigned id, const char* message)
{
    int success;
    char infoLog[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(id, 512, nullptr, infoLog);
        throw std::runtime_error(std::string("SHADER: ").append(message));
    }
}

void Shader::compile(const wchar_t* path, GLenum type)
{
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    file.open(getFileFullPath(path).c_str());

    std::stringstream stream;
    stream << file.rdbuf();
    file.close();
    std::string code = stream.str();

    unsigned id = glCreateShader(type);
    const char* tmp = code.c_str();
    glShaderSource(id, 1, &tmp, nullptr);
    glCompileShader(id);
    checkCompileErrors(id, "Can't compile shader");

    glAttachShader(m_id, id);
    glDeleteShader(id);

    checkCompileErrors(m_id, "Can't execute shader programm");
}
