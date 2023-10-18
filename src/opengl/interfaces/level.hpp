#pragma once
#ifndef GRAPHIC_LIBRARIES_OPENGL_INTERFACES_LEVEL_HPP_
#define GRAPHIC_LIBRARIES_OPENGL_INTERFACES_LEVEL_HPP_

#include <list>
#include <memory>

#include <glm/gtc/matrix_transform.hpp>

namespace GraphicLibraries
{
namespace OpenGL
{

namespace Windows { class GLFWWindow; }

namespace Interfaces
{
    class ICamera;

    class ILevel
    {
    public:
        ILevel() { }
        virtual ~ILevel() { }

        ILevel(const ILevel& other)            = delete;
        ILevel(ILevel&& other)                 = delete;
        ILevel& operator=(const ILevel& other) = delete;
        ILevel& operator=(ILevel&& other)      = delete;

        virtual void init(Windows::GLFWWindow* window) = 0;
        virtual void release()                         = 0;
        virtual void update(float dt)                  = 0;
        virtual void draw()                            = 0;

    protected:
        std::shared_ptr<ICamera> m_currentCamera = nullptr;
        std::list<std::shared_ptr<ICamera>> m_camerasList;
    };
}
}
}

#endif // GRAPHIC_LIBRARIES_OPENGL_INTERFACES_LEVEL_HPP_
