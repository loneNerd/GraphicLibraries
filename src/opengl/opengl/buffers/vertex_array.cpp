#include "vertex_array.hpp"

#include <GL/glew.h>

namespace Buffers = Engine::OpenGL::Buffers;

Buffers::VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_bufferID);
    glBindVertexArray(m_bufferID);
}

Buffers::VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_bufferID);
}

void Buffers::VertexArray::Bind()
{
    glBindVertexArray(m_bufferID);
}

void Buffers::VertexArray::Unbind()
{
    glBindVertexArray(0);
}

GLint Buffers::VertexArray::GetID()
{
    return m_bufferID;
}
