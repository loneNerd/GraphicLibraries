#pragma once
#ifndef GRAPHIC_LIBRARIES_OPENGL_INTERFACES_RESOURCE_HPP_
#define GRAPHIC_LIBRARIES_OPENGL_INTERFACES_RESOURCE_HPP_

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
    };
}
}
}

#endif // GRAPHIC_LIBRARIES_OPENGL_INTERFACES_RESOURCE_HPP_
