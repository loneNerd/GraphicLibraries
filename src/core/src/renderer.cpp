#include "core/renderer.hpp"

#include "opengl/resources/shader.hpp"
#include "core/resource_management/material_manager.hpp"
#include "core/components/material_renderer.hpp"

namespace Core = Engine::Core;
namespace OGL = Engine::OpenGL;
namespace OGLResources = Engine::OpenGL::Resources;
namespace CoreResources = Engine::Core::Resources;
namespace ResManagement = Engine::Core::ResourceManagement;

void Core::Renderer::RenderScene
(
    std::shared_ptr<Core::SceneSystem::Scene> scene,
    std::shared_ptr<Engine::OpenGL::Camera> camera,
    const Math::FVector3& cameraPosition,
    const Math::FQuaternion& cameraRotation,
    std::shared_ptr<Engine::OpenGL::Resources::Shader> shader
)
{
    for (std::shared_ptr<Core::Actor> actor : scene->GetActors())
    {
        std::shared_ptr<Components::ModelRenderer> modelRenderer = actor->GetComponent<Components::ModelRenderer>();
        if (modelRenderer)
        {
            if (std::shared_ptr<Engine::OpenGL::Resources::Model> model = modelRenderer->GetModel())
            {
                for (std::shared_ptr<OGLResources::Mesh> mesh : model->GetMeshes())
                {
                    DrawMesh(mesh, shader, camera);
                }
            }
        }
    }
}

void Core::Renderer::DrawMesh(std::shared_ptr<OGLResources::Mesh> mesh, std::shared_ptr<OGLResources::Shader> shader, std::shared_ptr<OGL::Camera> camera)
{
    std::shared_ptr<CoreResources::Material> material = ResManagement::MaterialManager::Instance().GetResource(mesh->GetMaterialName());

    uint8_t stateMask = material->GenerateStateMask();
    (material->HasDepthTest() ? glEnable : glDisable)(static_cast<GLenum>(OGL::Settings::ERenderingCapability::DepthTest));

    material->Bind(shader);
    mesh->Bind();

    shader->SetUniformMat4("model", Math::FMatrix4(1.f, 0.f, 0.f, 0.f,
                                                   0.f, 1.f, 0.f, 0.f,
                                                   0.f, 0.f, 1.f, 0.f,
                                                   0.f, 0.f, 0.f, 1.f));
    shader->SetUniformMat4("view", camera->GetViewMatrix());
    shader->SetUniformMat4("projection", camera->GetProjectionMatrix());

    glDrawElements(static_cast<GLenum>(OGL::Settings::EPrimitiveMode::Triangles), mesh->GetIndexCount(), GL_UNSIGNED_INT, nullptr);

    mesh->Unbind();
    material->UnBind(shader);
}
