#pragma once
#ifndef ENGINE_CONTEXT_HPP_
#define ENGINE_CONTEXT_HPP_

#include <filesystem>
#include <memory>
#include <string>

namespace Engine
{
    class Context
    {
    public:
        Context(const std::filesystem::path& projectPath);
        ~Context();

        Context(const Context& other)             = delete;
        Context(Context&& other)                  = delete;
        Context& operator=(const Context& other)  = delete;
        Context& operator=(const Context&& other) = delete;

        const std::filesystem::path ProjectPath;
        const std::filesystem::path ProjectAssetsPath;
        const std::filesystem::path ProjectShadersPath;
    };
}

#endif // ENGINE_CONTEXT_HPP_
