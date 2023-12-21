#pragma once
#ifndef GRAPHICLIBRARIES_RENDER_INTERFACE_H_
#define GRAPHICLIBRARIES_RENDER_INTERFACE_H_

namespace GraphicLibraries
{
namespace Windows { class IWindow; }
namespace Engines
{
    class IRender
    {
    public:
        IRender() { }
        virtual ~IRender() { }

        IRender(const IRender& other)            = delete;
        IRender(IRender&& other)                 = delete;
        IRender& operator=(const IRender& other) = delete;
        IRender& operator=(IRender&& other)      = delete;

        virtual void init()             = 0;
        virtual void release()          = 0;
        virtual void newFrame(float dt) = 0;

        virtual Windows::IWindow* getWindow() const = 0;

    protected:
        bool m_isInit = false;
    };
}
}

#endif // GRAPHICLIBRARIES_RENDER_INTERFACE_H_
