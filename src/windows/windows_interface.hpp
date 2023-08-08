#pragma once
#ifndef GRAPHICLIBRARIES_WINDOWS_INTERFACE_H_
#define GRAPHICLIBRARIES_WINDOWS_INTERFACE_H_

namespace GraphicLibraries
{
namespace Windows
{
    class IWindow
    {
    public:
        IWindow() : m_isClosed { true }, m_isInit{ false } { }
        virtual ~IWindow() { }

        IWindow(const IWindow& other)            = delete;
        IWindow(IWindow&& other)                 = delete;
        IWindow& operator=(const IWindow& other) = delete;
        IWindow& operator=(IWindow&& other)      = delete;

        virtual void init()        = 0;
        virtual void release()     = 0;
        virtual void handleEvent() = 0;

        virtual bool isClosed() { return m_isClosed; }

        virtual int getWidth() const  { return INITIAL_WIDTH; }
        virtual int getHeight() const { return INITIAL_HEIGHT; }

        const int INITIAL_WIDTH  = 1280;
        const int INITIAL_HEIGHT = 720;

    protected:
        bool m_isClosed;
        bool m_isInit;
    };
}
}

#endif // GRAPHICLIBRARIES_WINDOWS_INTERFACE_H_
