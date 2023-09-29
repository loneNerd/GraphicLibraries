#pragma once
#ifndef GRAPHICLIBRARIES_OPEN_GL_RESOURCE_HPP_
#define GRAPHICLIBRARIES_OPEN_GL_RESOURCE_HPP_

namespace GraphicLibraries
{
namespace Engines
{
    class IResource
    {
    public:
        IResource() { }
        virtual ~IResource() { }

        virtual void load( const char* path ) = 0;
        virtual void release() = 0;
    };
}
}

#endif // GRAPHICLIBRARIES_OPEN_GL_RESOURCE_HPP_
