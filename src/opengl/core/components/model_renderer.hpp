#pragma once
#ifndef ENGINE_CORE_COMPONENTS_MODEL_RENDERER_HPP_
#define ENGINE_CORE_COMPONENTS_MODEL_RENDERER_HPP_

#include "component.hpp"
#include "opengl/geometry/vertex.hpp"
#include "opengl/resources/model.hpp"

namespace Engine
{
namespace Core
{

class Actor;

namespace Components
{
    class ModelRenderer : public Component
    {
    public:
        ModelRenderer();

        ModelRenderer(const ModelRenderer& other)             = delete;
        ModelRenderer(ModelRenderer&& other)                  = delete;
        ModelRenderer& operator=(const ModelRenderer& other)  = delete;
        ModelRenderer& operator=(const ModelRenderer&& other) = delete;

        std::string GetName() override;
        void SetModel(std::shared_ptr<OpenGL::Resources::Model> model);
        std::shared_ptr<OpenGL::Resources::Model> GetModel() const;

    private:
        std::shared_ptr<OpenGL::Resources::Model> m_model = nullptr;
    };
}
}
}

#endif // ENGINE_CORE_COMPONENTS_MODEL_RENDERER_HPP_
