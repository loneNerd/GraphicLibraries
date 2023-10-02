#pragma once
#ifndef GRAPHICLIBRARIES_OPEN_GL_RENDER_FACTORY_H_
#define GRAPHICLIBRARIES_OPEN_GL_RENDER_FACTORY_H_

#include "opengl.hpp"
#include "windows/glfw.hpp"

namespace GraphicLibraries
{
namespace Engines
{
    static IRender* getNewEngine() { return new OpenGL; }
}

namespace Windows
{
    static IWindow* getNewWindow() { return new GLFWWindow; }
}
}

#endif // GRAPHICLIBRARIES_OPEN_GL_RENDER_FACTORY_H_
