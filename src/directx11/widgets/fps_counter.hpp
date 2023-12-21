#pragma once
#ifndef GRAPHICLIBRARIES_WIDGETS_FPS_COUNTER_H_
#define GRAPHICLIBRARIES_WIDGETS_FPS_COUNTER_H_

namespace GraphicLibraries
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

#endif // GRAPHICLIBRARIES_WIDGETS_FPS_COUNTER_H_
