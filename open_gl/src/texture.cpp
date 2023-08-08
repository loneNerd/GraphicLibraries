#include "texture.hpp"

#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace GraphicLibraries::Engines;

Texture::Texture()
{
    glGenTextures( 1, &m_id );
}

Texture::~Texture()
{
    glDeleteTextures( 1, &m_id );
}

void Texture::load( const char* path )
{
    stbi_set_flip_vertically_on_load( m_isFliped );

    // load image
    int nrChannels;
    unsigned char* data = stbi_load( path, &m_width, &m_height, &nrChannels, 0 );

    if ( !data )
        throw std::runtime_error( std::string( "Can't read texture file: " ).append( path ) );

    if ( nrChannels == 1 )
    {
        m_internalFormat = GL_RED;
        m_imageFormat    = GL_RED;
    }
    else if ( nrChannels == 3 )
    {
        m_internalFormat = GL_RGB;
        m_imageFormat    = GL_RGB;
    }
    else if ( nrChannels == 4 )
    {
        m_internalFormat = GL_RGBA;
        m_imageFormat    = GL_RGBA;
    }

    // create Texture
    glBindTexture( GL_TEXTURE_2D, m_id );

    // set Texture wrap and filter modes
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapS );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filterMin );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filterMax );

    glTexImage2D( GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_imageFormat, GL_UNSIGNED_BYTE, data );

    stbi_image_free( data );

    // unbind texture
    glBindTexture( GL_TEXTURE_2D, 0 );

    m_isLoaded = true;
}

void Texture::bind() const
{
    if ( !m_isLoaded )
        throw std::exception( "Texture is not loaded" );

    glBindTexture( GL_TEXTURE_2D, m_id );
}
