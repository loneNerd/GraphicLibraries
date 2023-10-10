#include "triangle.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <imgui/imgui.h>

#include "types/vertex.hpp"
#include "resource_manager.hpp"

using namespace GraphicLibraries::OpenGL::Objects;
using namespace GraphicLibraries::OpenGL::Types;
using namespace GraphicLibraries::OpenGL::Models2D;

void Triangle::init()
{
    m_currentColorType = EColorType::ESimpleTexture;

    m_shaders[EColorType::ESingleColor].load(L"shaders\\2d_vertex.glsl",
                                             L"shaders\\2d_single_color_fragment.glsl");

    m_shaders[EColorType::ESimpleTexture].load(L"shaders\\2d_vertex.glsl",
                                               L"shaders\\2d_simple_texture_fragment.glsl");

    Mesh newMesh { };

    newMesh.Textures.emplace_back(ResourceManager::loadResource(m_texturePath.c_str()));

    newMesh.Vertices =
    {
        { {  0.0f,  0.5f, 0.0f }, { 0.5f, 1.0f } },
        { {  0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f } },
        { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f } }
    };

    newMesh.Triangles = { 0, 1, 2 };

    glGenVertexArrays(1, &newMesh.VAO);
    glGenBuffers(1, &newMesh.VBO);
    glGenBuffers(1, &newMesh.EBO);

    glBindVertexArray(newMesh.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, newMesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, newMesh.Vertices.size() * sizeof(Vertex), newMesh.Vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, newMesh.Triangles.size() * sizeof(unsigned), newMesh.Triangles.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TextureCoord));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    m_meshes.emplace_back(newMesh);

    m_isInit = true;
}

void Triangle::release()
{
    for (auto& shader : m_shaders)
        shader.second.release();

    for (Mesh& mesh : m_meshes)
    {
        glDeleteVertexArrays(1, &mesh.VAO);
        glDeleteBuffers(1, &mesh.VBO);
        glDeleteBuffers(1, &mesh.EBO);

        for (std::shared_ptr<Texture> texture : mesh.Textures)
        {
            if (texture)
            {
                texture->release();
                texture = nullptr;
            }
        }

        mesh.Textures.clear();
    }

    m_isInit = false;
}

void Triangle::update(float dt)
{
    int color = static_cast<int>(m_currentColorType);
    ImGui::RadioButton("Single Color", &color, 0); ImGui::SameLine();
    ImGui::RadioButton("Texture", &color, 1);
    m_currentColorType = static_cast<EColorType>(color);

    if (m_currentColorType == EColorType::ESingleColor)
    {
        float simpleColor[4] = { m_color.x, m_color.y, m_color.z, m_color.w };
        if (ImGui::ColorEdit4("Color", simpleColor))
            m_color = glm::vec4(simpleColor[0], simpleColor[1], simpleColor[2], simpleColor[3]);
    }
    else if (m_currentColorType == EColorType::ESimpleTexture)
    {
        for (Mesh& mesh : m_meshes)
        {
            for (std::shared_ptr<Texture>& texture : mesh.Textures)
            {
                if (ImGui::Button("Open New Texture"))
                {
                    std::string newFile = openFile();

                    if (newFile != m_texturePath && newFile != "")
                    {
                        m_texturePath = newFile;
                        texture->release();
                        texture = nullptr;
                        texture = ResourceManager::loadResource(m_texturePath.c_str());
                    }
                }

                ImGui::Image((void*)(intptr_t)texture->getId(), ImVec2(192.0f, 128.0f));
            }
        }
    }


    float position[4] = { m_position.x, m_position.y, 0.0f, 0.0f };
    if (ImGui::InputFloat3("Position", position))
        m_position = glm::vec2(position[0], position[1]);

    float size[4] = { m_scale.x, m_scale.y, 0.0f, 0.0f };
    if (ImGui::InputFloat3("Size", size))
        m_scale = glm::vec2(size[0], size[1]);

    ImGui::InputFloat("Rotation", &m_rotation, 1.0f, m_rotation, "%.2f");
}

void Triangle::draw(const std::shared_ptr<Objects::Camera> camera)
{
    Shader& shader = m_shaders[m_currentColorType];
    shader.use();

    shader.setMatrix4("model", getModelMatrix());

    for (const Mesh& mesh : m_meshes)
    {
        if (m_currentColorType == EColorType::ESingleColor)
            shader.setVector4f("color", m_color);
        else if (m_currentColorType == EColorType::ESimpleTexture)
        {
            int i = 0;
            for (const std::shared_ptr<Texture>& texture : mesh.Textures)
            {
                glActiveTexture(GL_TEXTURE0 + i);
                shader.setInteger("diff", i);
                texture->bind();
                ++i;
            }
        }

        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned>(mesh.Triangles.size()), GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}
