#pragma once
#ifndef ENGINE_OPENGL_BUFFERS_VERTEX_ARRAY_HPP_
#define ENGINE_OPENGL_BUFFERS_VERTEX_ARRAY_HPP_

#include <memory>

#include "vertex_buffer.hpp"

namespace Engine::OpenGL::Buffers
{
    enum class EType
    {
        Byte          = 0x1400,
        UnisgnedByte  = 0x1401,
        Short         = 0x1402,
        UnsignedShort = 0x1403,
        Int           = 0x1404,
        UnsignedInt   = 0x1405,
        Float         = 0x1406,
        Double        = 0x140A
    };

    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        VertexArray(const VertexArray& other)             = delete;
        VertexArray(VertexArray&& other)                  = delete;
        VertexArray& operator=(const VertexArray& other)  = delete;
        VertexArray& operator=(const VertexArray&& other) = delete;

        template <class T>
        void BindAttribute(uint32_t attribute, std::shared_ptr<VertexBuffer<T>> vertexBuffer, EType type, uint64_t count, uint64_t stride, intptr_t offset);
        void Bind();
        void Unbind();
        GLint GetID();

    private:
        GLuint m_bufferID = 0;
    };
}

#include "vertex_array.inl"

#endif // ENGINE_OPENGL_BUFFERS_VERTEX_ARRAY_HPP_
