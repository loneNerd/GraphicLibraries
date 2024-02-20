#include <exception>
#include <iostream>
#include <memory>

#include "engine/application.hpp"

int main(int argc, char* argv[])
{
    std::unique_ptr<Engine::Application> app = nullptr;

    try
    {
        app = std::make_unique<Engine::Application>(std::filesystem::path(argv[0]).parent_path());
    }
    catch (std::exception e)
    {
        std::cerr << "ERROR OCCURED" << std::endl;
        std::cerr << e.what() << std::endl;
        return -1;
    }

    if (app)
        app->Run();

    return 0;
}
