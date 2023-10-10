#pragma once
#ifndef GRAPHIC_LIBRARIES_OPENGL_WINDOWS_GLFW_HPP_
#define GRAPHIC_LIBRARIES_OPENGL_WINDOWS_GLFW_HPP_

#include <list>

#include <glfw/glfw3.h>

namespace GraphicLibraries
{
namespace OpenGL
{
namespace Windows
{
    class IInputObserver;

    class GLFWWindow
    {
    public:
        GLFWWindow();
        ~GLFWWindow();

        GLFWWindow(const GLFWWindow& other)            = delete;
        GLFWWindow(GLFWWindow&& other)                 = delete;
        GLFWWindow& operator=(const GLFWWindow& other) = delete;
        GLFWWindow& operator=(GLFWWindow&& other)      = delete;

        bool isClosed() { return m_isClosed; }

        void initForOpenGL();
        void release();

        void handleEvent();

        bool getKeyStatus(int key);

        bool isLeftMouseBtn()   { return glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT); }
        bool isRightMouseBtn()  { return glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT); }
        bool isMiddleMouseBtn() { return glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_MIDDLE); }

        bool isCursorDisable() const { return m_isCursorDisable; }
        void showCursor();
        void hideCursor();
        void toggleCursor();

        GLFWwindow* getWindow() const { return m_window; }

        void setTitle(const char* title);

        int getWidth() const  { return INITIAL_WIDTH; }
        int getHeight() const { return INITIAL_HEIGHT; }

        void attachInputObserver(IInputObserver* observer)
        {
            if (observer)
                m_inputObserversList.emplace_back(observer);
        }

        void detachInputObserver(IInputObserver* observer)
        {
            if (observer)
                m_inputObserversList.remove(observer);
        }

        static const int INITIAL_WIDTH = 1280;
        static const int INITIAL_HEIGHT = 720;

    private:
        GLFWwindow* m_window = nullptr;

        std::list<IInputObserver*> m_inputObserversList;

        bool m_isCursorDisable;

        bool m_isClosed;
        bool m_isInit;
    };
}
}
}

#endif // GRAPHIC_LIBRARIES_OPENGL_WINDOWS_GLFW_HPP_
