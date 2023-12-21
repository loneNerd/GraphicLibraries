#pragma once
#ifndef GRAPHICLIBRARIES_WINDOWS_OBSERVERS_HPP_
#define GRAPHICLIBRARIES_WINDOWS_OBSERVERS_HPP_

namespace GraphicLibraries
{
namespace Windows
{
    class IInputObserver
    {
    public:
        virtual ~IInputObserver() { }
        virtual void mouseMovementCallback(float xpos, float ypos) { }
        virtual void keyPressedCallback(int button) { }
        virtual void keyReleaseCallback(int button) { }
        virtual void mouseButtonPressedCallback(int button) { }
        virtual void mouseButtonReleaseCallback(int button) { }
    };
}
}

#endif // GRAPHICLIBRARIES_WINDOWS_OBSERVERS_HPP_
