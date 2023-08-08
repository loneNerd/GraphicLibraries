#include "triangle.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <imgui/imgui.h>

#include "vertex.hpp"

using namespace GraphicLibraries::Engines;
using namespace GraphicLibraries::Engines::Objects2D;

void Triangle::init()
{
    m_currentColorType = EColorType::ESimpleTexture;

    m_shaders[EColorType::ESingleColor].load(L"shaders\\2d_vertex.glsl",
                                             L"shaders\\2d_single_color_fragment.glsl");

    m_shaders[EColorType::ESimpleTexture].load(L"shaders\\2d_vertex.glsl",
                                               L"shaders\\2d_simple_texture_fragment.glsl");

    m_texture.load(m_texturePath.c_str());

    Mesh newMesh { };

    newMesh.Vertices =
    {
        { {  0.0f,  0.5f, 0.0f }, { 0.5f, 1.0f } },
        { {  0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f } },
        { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f } }
    };

    glGenVertexArrays(1, &newMesh.VAO);
    glGenBuffers(1, &newMesh.VBO);

    glBindVertexArray(newMesh.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, newMesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, newMesh.Vertices.size() * sizeof(Vertex), newMesh.Vertices.data(), GL_STATIC_DRAW);

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
    }

    m_texture.release();

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
        if (ImGui::Button("Open New Texture"))
        {
            std::string newFile = openFile();

            if (newFile != m_texturePath)
            {
                m_texturePath = newFile;
                m_texture.release();
                m_texture.load(m_texturePath.c_str());
            }
        }

        ImGui::Image((void*)(intptr_t)m_texture.getId(), ImVec2(192.0f, 128.0f));
    }

}

void Triangle::draw()
{
    Shader& shader = m_shaders[m_currentColorType];
    shader.use();

    if (m_currentColorType == EColorType::ESingleColor)
        shader.setVector4f("color", m_color);
    else if (m_currentColorType == EColorType::ESimpleTexture)
    {
        glActiveTexture(GL_TEXTURE0 + 1);
        shader.setInteger("diff", 1);
        m_texture.bind();
    }

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(m_position, 0.0f));

    model = glm::scale(model, glm::vec3(m_size, 1.0f));

    model = glm::translate(model, glm::vec3(0.5f * m_size, 0.0f));
    model = glm::rotate(model, glm::radians(m_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * m_size, 0.0f));

    shader.setMatrix4("model", model);

    for (const Mesh& mesh : m_meshes)
        glBindVertexArray(mesh.VAO);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}
