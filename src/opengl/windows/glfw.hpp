#pragma once
#ifndef GRAPHIC_LIBRARIES_OPENGL_WINDOWS_GLFW_HPP_
#define GRAPHIC_LIBRARIES_OPENGL_WINDOWS_GLFW_HPP_

#include <list>
#include <map>
#include <memory>
#include <string>

#define GLFW_INCLUDE_NONE
#include <glfw/glfw3.h>

#include <GL/glew.h>

namespace GraphicLibraries
{
namespace OpenGL
{

namespace Interfaces { class ILevel; }
namespace Widgets { class FpsCounter; class Settings; }

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

        void init();
        void release();
        void handleEvent();
        void newFrame(float dt);

        static void setCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
        static void setKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void setMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

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
        void updateBlendStatus();
        void updateDepthTestStatus();

        GLFWwindow* m_window = nullptr;

        const char* m_glslVersion = "#version 460";

        bool m_isBlend     = true;
        bool m_isDepthTest = true;

        GLenum m_sfactor = GL_SRC_ALPHA;
        GLenum m_dfactor = GL_ONE_MINUS_SRC_ALPHA;
        std::map<std::string, GLenum> m_blendFunctions;

        std::string m_currentLevel = "";
        std::map<std::string, std::shared_ptr<Interfaces::ILevel>> m_levels;

        std::unique_ptr<Widgets::FpsCounter> m_fpsCounter = nullptr;
        std::unique_ptr<Widgets::Settings> m_settings = nullptr;

        std::list<IInputObserver*> m_inputObserversList;

        bool m_isCursorDisable;

        bool m_isClosed;
        bool m_isInit;
    };
}
}
}

#endif // GRAPHIC_LIBRARIES_OPENGL_WINDOWS_GLFW_HPP_
