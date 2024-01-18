#pragma once
#ifndef ENGINE_OPENGL_BUFFERS_INDEX_BUFFER_HPP_
#define ENGINE_OPENGL_BUFFERS_INDEX_BUFFER_HPP_

#include <vector>

namespace Engine
{
namespace OpenGL
{
namespace Buffers
{
    class IndexBuffer
    {
    public:
        IndexBuffer(unsigned int* data, size_t elements);
        IndexBuffer(std::vector<uint32_t>& data) : IndexBuffer(data.data(), data.size()) { }
        ~IndexBuffer();

        IndexBuffer(const IndexBuffer& other)             = delete;
        IndexBuffer(IndexBuffer&& other)                  = delete;
        IndexBuffer& operator=(const IndexBuffer& other)  = delete;
        IndexBuffer& operator=(const IndexBuffer&& other) = delete;

        void Bind();
        void Unbind();
        uint32_t GetID();

    private:
        uint32_t m_bufferID = 0;
    };
}
}
}

#endif // ENGINE_OPENGL_BUFFERS_INDEX_BUFFER_HPP_
