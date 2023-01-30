#pragma once
#ifndef GRAPHICLIBRARIES_DIRECTX_11_RENDER_FACTORY_H_
#define GRAPHICLIBRARIES_DIRECTX_11_RENDER_FACTORY_H_

#include "directx_11.h"
#include "windows/win32.h"

namespace GraphicLibraries
{
namespace Engines
{
    static IRender* getNewEngine() { return new DirectX11; }
}

namespace Windows
{
    static IWindow* getNewWindow() { return new Win32Window; }
}
}

#endif // GRAPHICLIBRARIES_DIRECTX_11_RENDER_FACTORY_H_
