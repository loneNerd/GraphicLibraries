#include "vertex_buffer.hpp"

#include <GL/glew.h>

namespace Buffers = Engine::OpenGL::Buffers;

template <class T>
inline Buffers::VertexBuffer<T>::VertexBuffer(T* data, size_t elements)
{
    glGenBuffers(1, &m_bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
    glBufferData(GL_ARRAY_BUFFER, elements * sizeof(T), data, GL_STATIC_DRAW);
}

template<class T>
inline Buffers::VertexBuffer<T>::~VertexBuffer()
{
    glDeleteBuffers(1, &m_bufferID);
}

template <class T>
inline void Buffers::VertexBuffer<T>::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
}

template <class T>
inline void Buffers::VertexBuffer<T>::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template <class T>
inline uint32_t Buffers::VertexBuffer<T>::GetID()
{
    return m_bufferID;
}
