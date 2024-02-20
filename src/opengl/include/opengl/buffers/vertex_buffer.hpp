#pragma once
#ifndef ENGINE_OPENGL_BUFFERS_VERTEX_BUFFER_HPP_
#define ENGINE_OPENGL_BUFFERS_VERTEX_BUFFER_HPP_

#include <vector>

namespace Engine::OpenGL::Buffers
{
    template <class T>
    class VertexBuffer
    {
    public:
        VertexBuffer(T* data, size_t elements);
        VertexBuffer(std::vector<T>& data) : VertexBuffer(data.data(), data.size()) { }
        ~VertexBuffer();

        VertexBuffer(const VertexBuffer& other)             = delete;
        VertexBuffer(VertexBuffer&& other)                  = delete;
        VertexBuffer& operator=(const VertexBuffer& other)  = delete;
        VertexBuffer& operator=(const VertexBuffer&& other) = delete;

        void Bind();
        void Unbind();
        uint32_t GetID();

    private:
        uint32_t m_bufferID = 0;
    };
}

#include "vertex_buffer.inl"

#endif // ENGINE_OPENGL_BUFFERS_VERTEX_BUFFER_HPP_
