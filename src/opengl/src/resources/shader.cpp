#include "opengl/resources/shader.hpp"
#include "opengl/resources/texture.hpp"

#include <GL/glew.h>

namespace FS = std::filesystem;
namespace Math = Engine::Math;
namespace Resources = Engine::OpenGL::Resources;

Resources::Shader::Shader(std::string name, FS::path vertex, FS::path fragment, std::optional<FS::path> geometry, uint32_t id)
    : VertexFileTrace(vertex),
      FragmentFileTrace(fragment),
      GeometryFileTrace(geometry),
      Name(name),
      Id(id)
{
    queryUniforms();
}

Resources::Shader::~Shader()
{
    glDeleteProgram(Id);
}

std::shared_ptr<Resources::Shader> Resources::Shader::createSharedPtr(std::string name, FS::path vertex, FS::path fragment, std::optional<FS::path> geometry, uint32_t id)
{
    Resources::Shader* shader = new Resources::Shader(name, vertex, fragment, geometry, id);
    return std::shared_ptr<Resources::Shader>(shader);
}

std::unique_ptr<Resources::Shader> Resources::Shader::createUniquePtr(std::string name, FS::path vertex, FS::path fragment, std::optional<FS::path> geometry, uint32_t id)
{
    Resources::Shader* shader = new Resources::Shader(name, vertex, fragment, geometry, id);
    return std::unique_ptr<Resources::Shader>(shader);
}

void Resources::Shader::Bind() const
{
    glUseProgram(Id);
}

void Resources::Shader::Unbind() const
{
    glUseProgram(0);
}

void Resources::Shader::SetUniformInt(const std::string& name, int value)
{
    glUniform1i(getUniformLocation(name), value);
}

void Resources::Shader::SetUniformFloat(const std::string& name, float value)
{
    glUniform1f(getUniformLocation(name), value);
}

void Resources::Shader::SetUniformVec2(const std::string& name, const Math::FVector2& vec2)
{
    glUniform2f(getUniformLocation(name), vec2.X, vec2.Y);
}

void Resources::Shader::SetUniformVec3(const std::string& name, const Math::FVector3& vec3)
{
    glUniform3f(getUniformLocation(name), vec3.X, vec3.Y, vec3.Z);
}

void Resources::Shader::SetUniformVec4(const std::string& name, const Math::FVector4& vec4)
{
    glUniform4f(getUniformLocation(name), vec4.X, vec4.Y, vec4.Z, vec4.W);
}

void Resources::Shader::SetUniformMat4(const std::string& name, const Math::FMatrix4& mat4)
{
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_TRUE, &mat4.Data[0]);
}

//void Resources::Shader::SetUniformMat4(const std::string& name, const glm::mat4& mat4)
//{
//    glUniformMatrix4fv(getUniformLocation(name), 1, false, glm::value_ptr(mat4));
//}

int Resources::Shader::GetUniformInt(const std::string& name)
{
    int value;
    glGetUniformiv(Id, getUniformLocation(name), &value);
    return value;
}

float Resources::Shader::GetUniformFloat(const std::string& name)
{
    float value;
    glGetUniformfv(Id, getUniformLocation(name), &value);
    return value;
}

Math::FVector2 Resources::Shader::GetUniformVec2(const std::string& name)
{
    GLfloat values[2];
    glGetUniformfv(Id, getUniformLocation(name), values);
    return reinterpret_cast<Math::FVector2&>(values);
}

Math::FVector3 Resources::Shader::GetUniformVec3(const std::string& name)
{
    GLfloat values[3];
    glGetUniformfv(Id, getUniformLocation(name), values);
    return reinterpret_cast<Math::FVector3&>(values);
}

Math::FVector4 Resources::Shader::GetUniformVec4(const std::string& name)
{
    GLfloat values[4];
    glGetUniformfv(Id, getUniformLocation(name), values);
    return reinterpret_cast<Math::FVector4&>(values);
}

Math::FMatrix4 Resources::Shader::GetUniformMat4(const std::string& name)
{
    GLfloat values[16];
    glGetUniformfv(Id, getUniformLocation(name), values);
    return reinterpret_cast<Math::FMatrix4&>(values);
}

uint32_t Resources::Shader::getUniformLocation(const std::string& name)
{
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
        return m_uniformLocationCache.at(name);

    const int location = glGetUniformLocation(Id, name.c_str());

    m_uniformLocationCache[name] = location;

    return location;
}

void Resources::Shader::queryUniforms()
{
    GLint numActiveUniforms = 0;
    Uniforms.clear();
    glGetProgramiv(Id, GL_ACTIVE_UNIFORMS, &numActiveUniforms);
    std::vector<GLchar> nameData(256);
    for (int unif = 0; unif < numActiveUniforms; ++unif)
    {
        GLint arraySize = 0;
        GLenum type = 0;
        GLsizei actualLength = 0;
        glGetActiveUniform(Id, unif, static_cast<GLsizei>(nameData.size()), &actualLength, &arraySize, &type, &nameData[0]);
        std::string name(static_cast<char*>(nameData.data()), actualLength);

        std::any defaultValue;
        switch (static_cast<UniformType>(type))
        {
            case UniformType::UniformBool:      defaultValue = std::make_any<bool>(GetUniformInt(name));            break;
            case UniformType::UniformInt:       defaultValue = std::make_any<int>(GetUniformInt(name));             break;
            case UniformType::UniformFloat:     defaultValue = std::make_any<float>(GetUniformFloat(name));         break;
            case UniformType::UniformFloatVec2: defaultValue = std::make_any<Math::FVector2>(GetUniformVec2(name)); break;
            case UniformType::UniformFloatVec3: defaultValue = std::make_any<Math::FVector3>(GetUniformVec3(name)); break;
            case UniformType::UniformFloatVec4: defaultValue = std::make_any<Math::FVector4>(GetUniformVec4(name)); break;
            case UniformType::UniformFloatMat4: defaultValue = std::make_any<Math::FMatrix4>(GetUniformMat4(name)); break;
            case UniformType::UniformSampler2D: defaultValue = std::make_any<int>(GetUniformInt(name));             break;
        }

        if (defaultValue.has_value())
        {
            Uniforms.push_back
            ({
                static_cast<UniformType>(type),
                name,
                getUniformLocation(nameData.data()),
                defaultValue
                });
        }
    }
}

std::optional<std::reference_wrapper<const Resources::UniformInfo>> Resources::Shader::GetUniformInfo(const std::string& name) const
{
    auto found = std::find_if(Uniforms.begin(), Uniforms.end(), [&name](const UniformInfo& element)
        {
            return name == element.Name;
        });

    if (found != Uniforms.end())
        return std::make_optional(*found);
    else
        return std::nullopt;
}
