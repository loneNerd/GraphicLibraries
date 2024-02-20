#pragma once
#ifndef ENGINE_APPLICATION_HPP_
#define ENGINE_APPLICATION_HPP_

#include "editor.hpp"
#include "context.hpp"

namespace Engine
{
    class Application
    {
    public:
        Application(const std::filesystem::path& projectPath) : m_context(projectPath), m_editor(m_context) { }
        ~Application() = default;

        Application(const Application& other)             = delete;
        Application(Application&& other)                  = delete;
        Application& operator=(const Application& other)  = delete;
        Application& operator=(const Application&& other) = delete;

        void Run();
        bool IsRunning() const;

    private:
        Context m_context;
        Editor m_editor;
    };
}

#endif // ENGINE_APPLICATION_HPP_
