#pragma once
#ifndef GRAPHIC_LIBRARIES_OPENGL_INTERFACES_RESOURCE_HPP_
#define GRAPHIC_LIBRARIES_OPENGL_INTERFACES_RESOURCE_HPP_

#include <string>

namespace GraphicLibraries
{
namespace OpenGL
{
namespace Interfaces
{
    class IResource
    {
    public:
        IResource() { }
        virtual ~IResource() { }

        IResource(const IResource& other)            = delete;
        IResource(IResource&& other)                 = delete;
        IResource& operator=(const IResource& other) = delete;
        IResource& operator=(IResource&& other)      = delete;

        virtual void load(const char* path) = 0;
        virtual void release() = 0;

        virtual const char* getPath() const { return m_path.c_str(); }

    protected:
        std::string m_path;
    };
}
}
}

#endif // GRAPHIC_LIBRARIES_OPENGL_INTERFACES_RESOURCE_HPP_
