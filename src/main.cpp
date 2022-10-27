#include <exception>

#include "engines/directx_11.h"
#include "engines/directx_12.h"

#include "ui/window_sdl2.h"

using namespace GraphicLibraries::UI;
using namespace GraphicLibraries::Engines;

int main(int argc, char* argv[])
{
    try
    {
        DirectX12 m_directx12;
        //DirectX11 m_directx11;

        std::cout << "Start rendering" << std::endl;
        while (WindowSDL2::getInstance().render())
            m_directx12.render();
            //m_directx11.render();
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
