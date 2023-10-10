#pragma once
#ifndef GRAPHIC_LIBRARIES_OPENGL_WIDGETS_FPS_COUNTER_HPP_
#define GRAPHIC_LIBRARIES_OPENGL_WIDGETS_FPS_COUNTER_HPP_

namespace GraphicLibraries
{
namespace OpenGL
{
namespace Widgets
{
    class FpsCounter
    {
    public:
        FpsCounter() { }
        ~FpsCounter() { }

        FpsCounter(const FpsCounter& other)            = delete;
        FpsCounter(FpsCounter&& other)                 = delete;
        FpsCounter& operator=(const FpsCounter& other) = delete;
        FpsCounter& operator=(FpsCounter&& other)      = delete;

        void draw();
    };
}
}
}

#endif // GRAPHIC_LIBRARIES_OPENGL_WIDGETS_FPS_COUNTER_HPP_
