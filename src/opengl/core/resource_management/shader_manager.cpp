#include "shader_manager.hpp"

#include <fstream>
#include <sstream>

#include <GL/glew.h>

#include "core/resource_management/resource_manager.hpp"
#include "tools/utils/path_parser.hpp"

namespace FS = std::filesystem;
namespace ResManagement = Engine::Core::ResourceManagement;
namespace OGLResources = Engine::OpenGL::Resources;
namespace Loaders = Engine::OpenGL::Resources::Loaders;
namespace Utils = Engine::Tools::Utils;

std::shared_ptr<OGLResources::Shader> ResManagement::ShaderManager::Create(std::filesystem::path vertexShader, std::filesystem::path fragmentShader, std::optional<std::filesystem::path> geometryShader)
{
    std::string name = Utils::PathParser::GetFileName(vertexShader) + Utils::PathParser::GetFileName(fragmentShader) + (geometryShader.has_value() ? Utils::PathParser::GetFileName(geometryShader.value()) : "");

    if (IsResourceLoaded(name))
        return GetResource(name);

    uint32_t programID = createProgram(vertexShader, fragmentShader, geometryShader);

    if (programID)
    {
        auto newShader = m_resources.emplace(name, OGLResources::Shader::createSharedPtr(name, vertexShader, fragmentShader, geometryShader, programID));
        return newShader.first->second;
    }

    return nullptr;
}

void ResManagement::ShaderManager::Recompile(std::shared_ptr<OGLResources::Shader> shader, std::filesystem::path vertexShader, std::filesystem::path fragmentShader, std::optional<std::filesystem::path> geometryShader)
{
    std::string name = shader->Name;
    glDeleteProgram(shader->Id);
    RemoveResource(name);
    std::shared_ptr<OGLResources::Shader> newShader = Create(vertexShader, fragmentShader, geometryShader);
    shader = newShader;
}

void ResManagement::ShaderManager::Destroy(const std::string& name)
{
    if (IsResourceLoaded(name) && m_resources[name].use_count() < 2)
    {
        RemoveResource(name);
    }
}

uint32_t ResManagement::ShaderManager::createProgram(std::filesystem::path vertexShader, std::filesystem::path fragmentShader, std::optional<std::filesystem::path> geometryShader)
{
    const uint32_t program = glCreateProgram();

    const uint32_t vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    const uint32_t fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    uint32_t gs = 0;
    if (geometryShader.has_value())
        gs = compileShader(GL_GEOMETRY_SHADER, geometryShader.value());

    if (vs == 0 || fs == 0)
        return 0;

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    if (geometryShader.has_value())
        glAttachShader(program, gs);

    glLinkProgram(program);

    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

    if (linkStatus == GL_FALSE)
    {
        GLint maxLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        std::string errorLog(maxLength, ' ');
        glGetProgramInfoLog(program, maxLength, &maxLength, errorLog.data());

        glDeleteProgram(program);

        return 0;
    }

    glValidateProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);

    if (geometryShader.has_value())
        glDeleteShader(gs);

    return program;
}

uint32_t ResManagement::ShaderManager::compileShader(uint32_t type, const FS::path& path)
{
    const uint32_t id = glCreateShader(type);

    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    file.open(path.string());

    std::stringstream stream;
    stream << file.rdbuf();
    file.close();
    std::string code = stream.str();
    const char* tmp = code.c_str();
    glShaderSource(id, 1, &tmp, nullptr);
    glCompileShader(id);

    GLint compileStatus;
    glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus);

    if (compileStatus == GL_FALSE)
    {
        GLint maxLength;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

        std::string errorLog(maxLength, ' ');
        glGetShaderInfoLog(id, maxLength, &maxLength, errorLog.data());

        std::string shaderTypeString = "VERTEX SHADER";

        if (type == GL_FRAGMENT_SHADER)
            shaderTypeString = "FRAGMENT SHADER";
        else if (type == GL_GEOMETRY_SHADER)
            shaderTypeString = "GEOMETRY SHADER";

        std::string errorHeader = "[" + shaderTypeString + "] \"";

        glDeleteShader(id);

        return 0;
    }

    return id;
}
