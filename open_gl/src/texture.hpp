#pragma once
#ifndef GRAPHICLIBRARIES_OPEN_GL_TEXTURE_HPP_
#define GRAPHICLIBRARIES_OPEN_GL_TEXTURE_HPP_

#include <gl/glew.h>

#include "resource.hpp"

namespace GraphicLibraries
{
namespace Engines
{
    enum class ETextureType
    {
        EDiffuse
        //ENormals
    };

    class Texture : public IResource
    {
    public:
        Texture();
        ~Texture();

        Texture(const Texture& other) = delete;
        Texture(Texture&& other) = delete;
        Texture& operator=(const Texture& other) = delete;
        Texture& operator=(Texture&& other) = delete;

        virtual void load(const char* path) override;
        virtual void release() override { }
        void bind() const;

        void flipTexture( bool flip ) { m_isFliped = flip; }

        unsigned getId() const { return m_id; }

        bool isFliped() const { return m_isFliped; }
        ETextureType getTextureType() const { return m_textureType; }

        unsigned getWrapS() const { return m_wrapS; }
        unsigned getWrapT() const { return m_wrapT; }

        unsigned getFilterMin() const { return m_filterMin; }
        unsigned getFilterMax() const { return m_filterMax; }

        void setTextureType(ETextureType type) { m_textureType = type; }

        void setWrapS( unsigned wrapS ) { m_wrapS = wrapS; }
        void setWrapT( unsigned wrapT ) { m_wrapT = wrapT; }

        void setFilterMin( unsigned filterMin ) { m_filterMin = filterMin; }
        void setFilterMax( unsigned filterMax ) { m_filterMax = filterMax; }

    private:
        // holds the ID of the texture object, used for all texture operations to reference to this particular texture
        unsigned m_id = 0;

        ETextureType m_textureType = ETextureType::EDiffuse;

        // texture image dimensions
        // width and height of loaded image in pixels
        int m_width  = 0;
        int m_height = 0;

        // texture Format
        unsigned m_internalFormat = GL_RGB; // format of texture object
        unsigned m_imageFormat    = GL_RGB; // format of loaded image

        // texture configuration
        unsigned m_wrapS = GL_REPEAT; // wrapping mode on S axis
        unsigned m_wrapT = GL_REPEAT; // wrapping mode on T axis

        unsigned m_filterMin = GL_LINEAR; // filtering mode if texture pixels < screen pixels
        unsigned m_filterMax = GL_LINEAR; // filtering mode if texture pixels > screen pixels

        bool m_isFliped = false;
        bool m_isLoaded = false;
    };
}
}

#endif // GRAPHICLIBRARIES_OPEN_GL_TEXTURE_HPP_
