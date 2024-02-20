#pragma once
#ifndef ENGINE_OPENGL_RESOURCES_MESH_HPP_
#define ENGINE_OPENGL_RESOURCES_MESH_HPP_

#include <filesystem>
#include <memory>
#include <vector>

#include "mesh_interface.hpp"
#include "opengl/buffers/index_buffer.hpp"
#include "opengl/buffers/vertex_array.hpp"
#include "opengl/buffers/vertex_buffer.hpp"
#include "opengl/geometry/vertex.hpp"

namespace Engine::OpenGL::Resources
{
    class Mesh : public IMesh
    {
    public:
        Mesh(const std::vector<Geometry::Vertex>& vertices, const std::vector<uint32_t>& indices, std::string materialName);
        ~Mesh();

        Mesh(const Mesh& other)             = delete;
        Mesh(Mesh&& other)                  = delete;
        Mesh& operator=(const Mesh& other)  = delete;
        Mesh& operator=(const Mesh&& other) = delete;

        virtual void Bind() override;
        virtual void Unbind() override;
        virtual uint32_t GetVertexCount() override;
        virtual uint32_t GetIndexCount() override;
        const std::string& GetMaterialName() const;

    private:
        void createBuffers(const std::vector<Geometry::Vertex>& vertices, const std::vector<uint32_t>& indices);

        const uint32_t m_vertexCount;
        const uint32_t m_indicesCount;

        const std::string m_materialName;

        Buffers::VertexArray                          m_vertexArray;
        std::shared_ptr<Buffers::VertexBuffer<float>> m_vertexBuffer = nullptr;
        std::unique_ptr<Buffers::IndexBuffer>         m_indexBuffer = nullptr;
    };
}

#endif // ENGINE_OPENGL_RESOURCES_MESH_HPP_
