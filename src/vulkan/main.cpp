#include <chrono>
#include <exception>
#include <iostream>
#include <memory>

#include "engines/render_interface.hpp"
#include "windows/windows_interface.hpp"
#include "render_factory.hpp"

using namespace GraphicLibraries::Engines;
using namespace GraphicLibraries::Windows;

typedef std::chrono::high_resolution_clock Clock;

int main(int argc, char* argv[])
{
    try
    {
        std::unique_ptr<IRender> render = std::unique_ptr<IRender>(getNewEngine());
        render->init();

        IWindow* window = render->getWindow();

        float deltaTime = 0.0;
        Clock::time_point currentFrame = Clock::now();
        Clock::time_point lastFrame = currentFrame;

        for(; !window->isClosed(); window->handleEvent())
        {
            currentFrame = Clock::now();
            deltaTime = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(currentFrame - lastFrame).count();
            lastFrame = currentFrame;

            render->newFrame(deltaTime);
        }

        render->release();
        render = nullptr;
    }
    catch (std::exception e)
    {
        std::cerr << "ERROR OCCURED" << std::endl;
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}
