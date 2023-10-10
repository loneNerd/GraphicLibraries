#pragma once
#ifndef GRAPHIC_LIBRARIES_OPENGL_RECOURCE_MANAGER_HPP_
#define GRAPHIC_LIBRARIES_OPENGL_RECOURCE_MANAGER_HPP_

#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>

#include "objects/texture.hpp"

namespace GraphicLibraries
{
namespace OpenGL
{
    namespace Interfaces { class IResource; }

    class ResourceManager
    {
    public:
        ResourceManager()  = delete;
        ~ResourceManager() = delete;

        ResourceManager(const ResourceManager& other)             = delete;
        ResourceManager(ResourceManager&& other)                  = delete;
        ResourceManager& operator=(const ResourceManager& other)  = delete;
        ResourceManager& operator=(const ResourceManager&& other) = delete;

        //// Parameter "keep" don't remove resource from memory while no one is referencing it.
        //// Those resources should release only in clear() function.
        //template <typename Model>
        //static std::shared_ptr< Model > loadResource( const char* file, bool keep = false )
        //{
        //    static_assert( std::is_base_of<Engine::Interfaces::IResource, Model>::value, "Resource file should inherit Resource class" );

        //    if ( m_resources.find( file ) != m_resources.end() )
        //        return std::static_pointer_cast< Model >( m_resources[ file ] );

        //    std::shared_ptr< Model > res = std::make_shared< Model >();
        //    res->load( file );

        //    m_resources.emplace( file, res );

        //    if ( !keep )
        //        m_garbage.emplace_back( file );

        //    return std::static_pointer_cast< Model >( m_resources[ file ] );
        //}

        static std::shared_ptr<Objects::Texture> loadResource(const char* file, bool flip = false, bool keep = false)
        {
            if (m_resources.find(file) != m_resources.end())
                return std::static_pointer_cast<Objects::Texture>(m_resources[file]);

            std::shared_ptr<Objects::Texture> texture = std::make_shared<Objects::Texture>();
            texture->flipTexture(flip);
            texture->load(file);

            m_resources.emplace(file, texture);

            if (!keep)
                m_garbage.emplace_back(file);

            return std::static_pointer_cast<Objects::Texture>(m_resources[file]);
        }

        static void collectGarbage();
        static void clear();

    private:
        static std::unordered_map<std::string, std::shared_ptr<Interfaces::IResource>> m_resources;
        static std::list<std::string> m_garbage;
    };
}
}

#endif // GRAPHIC_LIBRARIES_OPENGL_RECOURCE_MANAGER_HPP_
