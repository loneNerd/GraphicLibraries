#pragma once
#ifndef GRAPHICLIBRARIES_VULKAN_RENDER_FACTORY_H_
#define GRAPHICLIBRARIES_VULKAN_RENDER_FACTORY_H_

#include "vulkan.hpp"
#include "windows/sdl2.hpp"

namespace GraphicLibraries
{
namespace Engines
{
    static IRender* getNewEngine() { return new Vulkan; }
}

namespace Windows
{
    static IWindow* getNewWindow() { return new SDL2Window; }
}
}

#endif // GRAPHICLIBRARIES_VULKAN_RENDER_FACTORY_H_
