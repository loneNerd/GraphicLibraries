#include "opengl/resources/mesh.hpp"

#include <algorithm>

namespace FS = std::filesystem;
namespace OGLResources = Engine::OpenGL::Resources;

OGLResources::Mesh::Mesh(const std::vector<Geometry::Vertex>& vertices, const std::vector<uint32_t>& indices, std::string materialName)
    : m_vertexCount(static_cast<uint32_t>(vertices.size())),
      m_indicesCount(static_cast<uint32_t>(indices.size())),
      m_materialName(materialName)
{
    createBuffers(vertices, indices);
}

OGLResources::Mesh::~Mesh()
{
    if (m_vertexBuffer)
        m_vertexBuffer = nullptr;

    if (m_indexBuffer)
        m_indexBuffer = nullptr;
}

void OGLResources::Mesh::Bind()
{
    m_vertexArray.Bind();
}

void OGLResources::Mesh::Unbind()
{
    m_vertexArray.Unbind();
}

uint32_t OGLResources::Mesh::GetVertexCount()
{
    return m_vertexCount;
}

uint32_t OGLResources::Mesh::GetIndexCount()
{
    return m_indicesCount;
}

const std::string& OGLResources::Mesh::GetMaterialName() const
{
    return m_materialName;
}

void OGLResources::Mesh::createBuffers(const std::vector<Geometry::Vertex>& vertices, const std::vector<uint32_t>& indices)
{
    std::vector<float> vertexData;
    std::vector<unsigned int> rawIndices;

    for (const auto& vertex : vertices)
    {
        vertexData.push_back(vertex.Position[0]);
        vertexData.push_back(vertex.Position[1]);
        vertexData.push_back(vertex.Position[2]);

        vertexData.push_back(vertex.TexCoords[0]);
        vertexData.push_back(vertex.TexCoords[1]);

        vertexData.push_back(vertex.Normals[0]);
        vertexData.push_back(vertex.Normals[1]);
        vertexData.push_back(vertex.Normals[2]);

        vertexData.push_back(vertex.Tangent[0]);
        vertexData.push_back(vertex.Tangent[1]);
        vertexData.push_back(vertex.Tangent[2]);

        vertexData.push_back(vertex.Bitangent[0]);
        vertexData.push_back(vertex.Bitangent[1]);
        vertexData.push_back(vertex.Bitangent[2]);
    }

    m_vertexBuffer = std::make_shared<Buffers::VertexBuffer<float>>(vertexData);
    m_indexBuffer = std::make_unique<Buffers::IndexBuffer>(const_cast<uint32_t*>(indices.data()), indices.size());

    uint64_t vertexSize = sizeof(Geometry::Vertex);

    m_vertexArray.BindAttribute(0, m_vertexBuffer, Buffers::EType::Float, 3, vertexSize, offsetof(Geometry::Vertex, Position));
    m_vertexArray.BindAttribute(1, m_vertexBuffer, Buffers::EType::Float, 2, vertexSize, offsetof(Geometry::Vertex, TexCoords));
    m_vertexArray.BindAttribute(2, m_vertexBuffer, Buffers::EType::Float, 3, vertexSize, offsetof(Geometry::Vertex, Normals));
    m_vertexArray.BindAttribute(3, m_vertexBuffer, Buffers::EType::Float, 3, vertexSize, offsetof(Geometry::Vertex, Tangent));
    m_vertexArray.BindAttribute(4, m_vertexBuffer, Buffers::EType::Float, 3, vertexSize, offsetof(Geometry::Vertex, Bitangent));
}
