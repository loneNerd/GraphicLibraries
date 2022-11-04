#include <exception>

#include "engines/directx_11.h"
#include "engines/directx_12.h"
#include "engines/vulkan.h"

#include "ui/window_sdl2.h"

using namespace GraphicLibraries::UI;
using namespace GraphicLibraries::Engines;

int main(int argc, char* argv[])
{
    try
    {
        Vulkan vulkan;

        std::cout << "Start rendering" << std::endl;
        while (WindowSDL2::getInstance().render())
            vulkan.render();

        std::cout << "Finish rendering" << std::endl;
    }
    catch (std::exception e)
    {
        std::cerr << "ERROR OCCURED" << std::endl;
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}
