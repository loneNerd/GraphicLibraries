#include <exception>
#include <iostream>
#include <memory>

#include "engines/render_interface.h"
#include "windows/windows_interface.h"
#include "render_factory.h"

using namespace GraphicLibraries::Engines;
using namespace GraphicLibraries::Windows;

int main(int argc, char* argv[])
{
    try
    {
        std::unique_ptr<IRender> render = std::unique_ptr<IRender>(getNewEngine());
        render->init();

        IWindow* window = render->getWindow();

        std::cout << "Start rendering" << std::endl;
        for(; !window->isClosed(); window->handleEvent())
        {
            render->newFrame();
        }

        std::cout << "Finish rendering" << std::endl;
        
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
