#pragma once
#ifndef GRAPHIC_LIBRARIES_OPENGL_WINDOWS_WINDOW_OBSERVERS_HPP_
#define GRAPHIC_LIBRARIES_OPENGL_WINDOWS_WINDOW_OBSERVERS_HPP_

namespace GraphicLibraries
{
namespace OpenGL
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
}

#endif // GRAPHIC_LIBRARIES_OPENGL_WINDOWS_WINDOW_OBSERVERS_HPP_
