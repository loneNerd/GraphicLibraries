#pragma once
#ifndef GRAPHICLIBRARIES_WINDOWS_INTERFACE_HPP_
#define GRAPHICLIBRARIES_WINDOWS_INTERFACE_HPP_

#include <algorithm>
#include <list>

namespace GraphicLibraries
{
namespace Windows
{
    class IInputObserver;

    class IWindow
    {
    public:
        IWindow() : m_isClosed{ true }, m_isInit{ false }, m_isCursorDisable{ false } { }
        virtual ~IWindow() { }

        IWindow(const IWindow& other)            = delete;
        IWindow(IWindow&& other)                 = delete;
        IWindow& operator=(const IWindow& other) = delete;
        IWindow& operator=(IWindow&& other)      = delete;

        virtual void init()        = 0;
        virtual void release()     = 0;
        virtual void handleEvent() = 0;

        virtual bool isClosed() { return m_isClosed; }

        virtual bool getKeyStatus(int key) { return false; }

        virtual bool isLeftMouseBtn() { return false; }
        virtual bool isRightMouseBtn() { return false; }
        virtual bool isMiddleMouseBtn() { return false; }

        virtual bool isCursorDisable() const { return m_isCursorDisable; }
        virtual void showCursor() { }
        virtual void hideCursor() { }
        virtual void toggleCursor() { }

        virtual int getWidth() const  { return INITIAL_WIDTH; }
        virtual int getHeight() const { return INITIAL_HEIGHT; }

        virtual void attachInputObserver(IInputObserver* observer)
        {
            if (observer)
                m_inputObserversList.emplace_back(observer);
        }

        virtual void detachInputObserver(IInputObserver* observer)
        {
            if (observer)
                m_inputObserversList.remove(observer);
        }

        static const int INITIAL_WIDTH  = 1280;
        static const int INITIAL_HEIGHT = 720;

    protected:
        std::list<IInputObserver*> m_inputObserversList;

        bool m_isCursorDisable;

        bool m_isClosed;
        bool m_isInit;
    };
}
}

#endif // GRAPHICLIBRARIES_WINDOWS_INTERFACE_HPP_
