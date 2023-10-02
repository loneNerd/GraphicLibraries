#pragma once
#ifndef GRAPHICLIBRARIES_WINDOWS_GLFW_H_
#define GRAPHICLIBRARIES_WINDOWS_GLFW_H_

#pragma comment(lib, "glfw3.lib")

#include <glfw/glfw3.h>

#include "windows_interface.hpp"

namespace GraphicLibraries
{
namespace Windows
{
    class GLFWWindow : public IWindow
    {
    public:
        GLFWWindow();
        virtual ~GLFWWindow() override;

        GLFWWindow(const GLFWWindow& other)            = delete;
        GLFWWindow(GLFWWindow&& other)                 = delete;
        GLFWWindow& operator=(const GLFWWindow& other) = delete;
        GLFWWindow& operator=(GLFWWindow&& other)      = delete;

        virtual void initForOpenGL();
        virtual void release() override;

        virtual void handleEvent() override;

        GLFWwindow* getWindow()     const { return m_window; }

        void setTitle(const char* title);

    private:
        virtual void init() override { }

        GLFWwindow* m_window = nullptr;
    };
}
}

#endif // GRAPHICLIBRARIES_WINDOWS_GLFW_H_
