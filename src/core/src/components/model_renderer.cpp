#include "core/components/model_renderer.hpp"

namespace OGLResources = Engine::OpenGL::Resources;
namespace Components = Engine::Core::Components;

std::string Components::ModelRenderer::GetName()
{
    return "Model Renderer";
}

void Components::ModelRenderer::SetModel(std::shared_ptr<OGLResources::Model> model)
{
    m_model = model;
}

std::shared_ptr<OGLResources::Model> Components::ModelRenderer::GetModel() const
{
    return m_model;
}
