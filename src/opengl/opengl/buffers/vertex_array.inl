#include "vertex_array.hpp"

#include <GL/glew.h>

namespace Buffers = Engine::OpenGL::Buffers;

template <class T>
inline void Buffers::VertexArray::BindAttribute(uint32_t attribute, std::shared_ptr<VertexBuffer<T>> vertexBuffer, EType type, uint64_t count, uint64_t stride, intptr_t offset)
{
    Bind();
    vertexBuffer->Bind();
    glEnableVertexAttribArray(attribute);
    glVertexAttribPointer(static_cast<GLuint>(attribute), static_cast<GLint>(count), static_cast<GLenum>(type), GL_FALSE, static_cast<GLsizei>(stride), reinterpret_cast<const GLvoid*>(offset));
}
