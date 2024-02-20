#pragma once
#ifndef ENGINE_CORE_RESOURCES_PARSERS_MODEL_PARSER_INTERFACE_HPP_
#define ENGINE_CORE_RESOURCES_PARSERS_MODEL_PARSER_INTERFACE_HPP_

#include <filesystem>
#include <memory>
#include <string>

#include "core/resources/parsers/assimp_parser_flags.hpp"

namespace Engine::OpenGL::Resources { class Model; }

namespace Engine::Core::Resources::Parsers
{
    class IModelParser
    {
    public:
        virtual bool LoadModel(const std::filesystem::path& filePath, std::shared_ptr<OpenGL::Resources::Model>& model, EAssimpParserFlags assimpFlags) = 0;
    };
}

#endif // ENGINE_CORE_RESOURCES_PARSERS_MODEL_PARSER_INTERFACE_HPP_
