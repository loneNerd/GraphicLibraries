#pragma once
#ifndef ENGINE_OPENGL_RESOURCES_PARSERS_MODEL_PARSER_INTERFACE_HPP_
#define ENGINE_OPENGL_RESOURCES_PARSERS_MODEL_PARSER_INTERFACE_HPP_

#include <filesystem>
#include <memory>
#include <string>

#include "model_parser_flags.hpp"

namespace Engine
{
namespace OpenGL
{
namespace Resources
{

class Model;

namespace Parsers
{
    class IModelParser
    {
    public:
        virtual bool LoadModel(const std::filesystem::path& filePath, std::shared_ptr<Model>& model, EModelParserFlags parserFlags) = 0;
    };
}
}
}
}

#endif // ENGINE_OPENGL_RESOURCES_PARSERS_MODEL_PARSER_INTERFACE_HPP_
