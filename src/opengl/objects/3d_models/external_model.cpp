#include "external_model.hpp"

#include <imgui/imgui.h>

#include "interfaces/camera.hpp"
#include "objects/external_model_loader.hpp"
#include "objects/texture.hpp"
#include "resource_manager.hpp"
#include "types/mesh.hpp"

using namespace GraphicLibraries::OpenGL::Interfaces;
using namespace GraphicLibraries::OpenGL::Objects;
using namespace GraphicLibraries::OpenGL::Objects::Models3D;
using namespace GraphicLibraries::OpenGL::Types;


void ExternalModel::init(const char* path)
{
    m_shader.load(L"shaders\\3d_vertex.glsl",
                  L"shaders\\3d_simple_texture_fragment.glsl");

    ExternalModelLoader loader;
    m_meshes = loader.load(getFileFullPath(path).c_str());

    for (Mesh& mesh : m_meshes)
    {
        glGenVertexArrays(1, &mesh.VAO);
        glGenBuffers(1, &mesh.VBO);
        glGenBuffers(1, &mesh.EBO);

        glBindVertexArray(mesh.VAO);

        glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);

        glBufferData(GL_ARRAY_BUFFER, mesh.Vertices.size() * sizeof(Vertex), mesh.Vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.Triangles.size() * sizeof(unsigned), mesh.Triangles.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        // vertex texture coords
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TextureCoord));
        //// vertex normals
        //glEnableVertexAttribArray(2);
        //glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        //// vertex tangent
        //glEnableVertexAttribArray(3);
        //glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        //// vertex bitangent
        //glEnableVertexAttribArray(4);
        //glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
        //// ids
        //glEnableVertexAttribArray(5);
        //glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, BoneIDs));

        //// weights
        //glEnableVertexAttribArray(6);
        //glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Weights));
        glBindVertexArray(0);
    }

    m_isInit = true;
}

void ExternalModel::release()
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

void ExternalModel::updateUI()
{
    ImGui::NewLine();

    float position[4] = { m_position.x, m_position.y, m_position.z, 0.0f };
    if (ImGui::InputFloat3("Position", position))
        m_position = glm::vec3(position[0], position[1], position[2]);

    float scale[4] = { m_scale.x, m_scale.y, m_scale.z, 0.0f };
    if (ImGui::InputFloat3("Scale", scale))
        m_scale = glm::vec3(scale[0], scale[1], scale[2]);

    float rotation[4] = { glm::degrees(m_rotation.x), glm::degrees(m_rotation.y), glm::degrees(m_rotation.z), 0.0f };
    if (ImGui::InputFloat3("Rotation", rotation))
        m_rotation = glm::vec3(glm::radians(rotation[0]), glm::radians(rotation[1]), glm::radians(rotation[2]));

    ImGui::NewLine();

    if (ImGui::CollapsingHeader("Meshes"))
    {
        ImGui::NewLine();

        int meshCounter = 0;
        for (Mesh& mesh : m_meshes)
        {
            ++meshCounter;
            ImGui::Text("Mesh %d: ", meshCounter);
            ImGui::Text("VAO: %d", mesh.VAO);
            ImGui::Text("VBO: %d", mesh.VBO);
            ImGui::Text("EBO: %d", mesh.EBO);
            ImGui::Text("Vertices Count: %d", mesh.Vertices.size());
            ImGui::Text("Textures Count: %d", mesh.Textures.size());
            ImGui::Text("Triangles Count: %d", mesh.Triangles.size());

            ImGui::NewLine();

            if (ImGui::CollapsingHeader(std::string("Textures##").append(std::to_string(meshCounter)).c_str()))
            {
                for (std::shared_ptr<Texture>& texture : mesh.Textures)
                {
                    ImGui::SeparatorText(texture->getPath());

                    if (ImGui::Button("Open New Texture"))
                    {
                        std::string newFile = openFile();

                        if (newFile != texture->getPath() && newFile != "")
                        {
                            texture->release();
                            texture = nullptr;
                            texture = ResourceManager::loadResource(newFile.c_str());
                        }
                    }

                    ImGui::Image((void*)(intptr_t)texture->getId(), ImVec2(128.0f, 128.0f));

                    ImGui::NewLine();
                }
            }
        }
    }

    ImGui::NewLine();
}

void ExternalModel::update(float dt)
{

}

void ExternalModel::draw(const std::shared_ptr<Interfaces::ICamera> camera)
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
