#pragma once
#ifndef GRAPHIC_LIBRARIES_OPENGL_INTERFACES_LEVEL_HPP_
#define GRAPHIC_LIBRARIES_OPENGL_INTERFACES_LEVEL_HPP_

#include <map>
#include <memory>
#include <string>

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
        virtual void updateUI()                        = 0;
        virtual void update(float dt)                  = 0;
        virtual void draw()                            = 0;

    protected:
        std::string m_currentCamera = "";
        std::map<std::string, std::shared_ptr<ICamera>> m_cameras;
    };
}
}
}

#endif // GRAPHIC_LIBRARIES_OPENGL_INTERFACES_LEVEL_HPP_
