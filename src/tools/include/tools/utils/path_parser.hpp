#pragma once
#ifndef ENGINE_TOOLS_UTILS_PATH_PARSER_HPP_
#define ENGINE_TOOLS_UTILS_PATH_PARSER_HPP_

#include <filesystem>
#include <string>

namespace Engine::Tools:: Utils
{
    class PathParser
    {
    public:
        PathParser() = delete;

        static std::string GetFileName(const std::filesystem::path& path);
    };
}

#endif // ENGINE_TOOLS_UTILS_PATH_PARSER_HPP_
