#pragma once
#ifndef GRAPHICLIBRARIES_VULKAN_RENDER_FACTORY_H_
#define GRAPHICLIBRARIES_VULKAN_RENDER_FACTORY_H_

#include "vulkan.h"
#include "windows/sdl2.h"

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
