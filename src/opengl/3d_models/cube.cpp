#include "cube.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <imgui/imgui.h>

#include "objects/camera.hpp"
#include "resource_manager.hpp"
#include "types/vertex.hpp"

using namespace GraphicLibraries::OpenGL::Objects;
using namespace GraphicLibraries::OpenGL::Types;
using namespace GraphicLibraries::OpenGL::Models3D;

void Cube::init()
{
    m_shader.load(L"shaders\\3d_vertex.glsl",
                  L"shaders\\3d_simple_texture_fragment.glsl");

    Mesh newMesh { };

    newMesh.Textures.emplace_back(ResourceManager::loadResource(m_texturePath.c_str()));

    newMesh.Vertices =
    {
        { { -0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f } },
        { {  0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f } },
        { {  0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f } },
        { { -0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f } },

        { { -0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f } },
        { {  0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f } },
        { {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f } },
        { { -0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f } },

        { { -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f } },
        { { -0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f } },
        { { -0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f } },
        { { -0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f } },

        { {  0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f } },
        { {  0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f } },
        { {  0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f } },
        { {  0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f } },

        { {  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f } },
        { { -0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f } },

        { { -0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f } },
        { {  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f } }
    };

    newMesh.Triangles =
    {
        0,  1,  2,
        2,  3,  0,
        4,  5,  6,
        6,  7,  4,
        8,  9, 10,
       10, 11,  8,
       12, 13, 14,
       14, 15, 12,
        8, 13, 16,
       16, 17,  8,
       18, 19, 14,
       14, 11, 18
    };

    glGenVertexArrays(1, &newMesh.VAO);
    glGenBuffers(1, &newMesh.VBO);
    glGenBuffers(1, &newMesh.EBO);

    glBindVertexArray(newMesh.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, newMesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, newMesh.Vertices.size() * sizeof(Vertex), newMesh.Vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, newMesh.Triangles.size() * sizeof(unsigned), newMesh.Triangles.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TextureCoord));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    m_meshes.emplace_back(newMesh);

    m_isInit = true;
}

void Cube::release()
{
    m_shader.release();

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

void Cube::update(float dt)
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

            ImGui::Image((void*)(intptr_t)texture->getId(), ImVec2(128.0f, 128.0f));
        }
    }

    float position[4] = { m_position.x, m_position.y, m_position.z, 0.0f };
    if (ImGui::InputFloat3("Position", position))
        m_position = glm::vec3(position[0], position[1], position[2]);

    float scale[4] = { m_scale.x, m_scale.y, m_scale.z, 0.0f };
    if (ImGui::InputFloat3("Scale", scale))
        m_scale = glm::vec3(scale[0], scale[1], scale[2]);

    float rotation[4] = { glm::degrees(m_rotation.x), glm::degrees(m_rotation.y), glm::degrees(m_rotation.z), 0.0f };
    if (ImGui::InputFloat3("Rotation", rotation))
        m_rotation = glm::vec3(glm::radians(rotation[0]), glm::radians(rotation[1]), glm::radians(rotation[2]));
}

void Cube::draw(const std::shared_ptr<Objects::Camera> camera)
{
    m_shader.use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_position);
    model *= glm::mat4_cast(glm::quat(m_rotation));
    model = glm::scale(model, m_scale);

    m_shader.setMatrix4("model", model);
    m_shader.setMatrix4("view", camera->getViewMatrix());
    m_shader.setMatrix4("projection", camera->getProjection());

    for (const Mesh& mesh : m_meshes)
    {
        int i = 0;
        for (const std::shared_ptr<Texture>& texture : mesh.Textures)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            m_shader.setInteger("diff", i);
            texture->bind();
            ++i;
        }

        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned>(mesh.Triangles.size()), GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}
