#include <chrono>
#include <exception>
#include <iostream>
#include <memory>

#include "objects/opengl.hpp"
#include "windows/glfw.hpp"

using namespace GraphicLibraries::OpenGL::Objects;
using namespace GraphicLibraries::OpenGL::Windows;

typedef std::chrono::high_resolution_clock Clock;

int main(int argc, char* argv[])
{
    try
    {
        std::unique_ptr<OpenGL> openGL = std::make_unique<OpenGL>();
        openGL->init();

        std::shared_ptr<GLFWWindow> window = openGL->getWindow();

        float deltaTime = 0.0;
        Clock::time_point currentFrame = Clock::now();
        Clock::time_point lastFrame = currentFrame;

        for(; !window->isClosed(); window->handleEvent())
        {
            currentFrame = Clock::now();
            deltaTime = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(currentFrame - lastFrame).count();
            lastFrame = currentFrame;

            openGL->newFrame(deltaTime);
        }

        openGL->release();
        openGL = nullptr;
    }
    catch (std::exception e)
    {
        std::cerr << "ERROR OCCURED" << std::endl;
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}
