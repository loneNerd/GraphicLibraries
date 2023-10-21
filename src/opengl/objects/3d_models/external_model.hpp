#pragma once
#ifndef GRAPHIC_LIBRARIES_OPENGL_3D_MODELS_EXTERNAL_MODEL_HPP_
#define GRAPHIC_LIBRARIES_OPENGL_3D_MODELS_EXTERNAL_MODEL_HPP_

#include <memory>

#include "interfaces/drawable.hpp"
#include "interfaces/object_3d.hpp"
#include "objects/shader.hpp"
#include "utils.hpp"

namespace GraphicLibraries
{
namespace OpenGL
{
namespace Objects
{
namespace Models3D
{
    class ExternalModel : public Interfaces::IDrawable, public Interfaces::IObject3D
    {
    public:
        ExternalModel() { }
        virtual ~ExternalModel() { if (m_isInit) release(); }

        ExternalModel(const ExternalModel& other)            = delete;
        ExternalModel(ExternalModel&& other)                 = delete;
        ExternalModel& operator=(const ExternalModel& other) = delete;
        ExternalModel& operator=(ExternalModel&& other)      = delete;

        virtual void init(const char* path);
        virtual void release() override;
        virtual void updateUI() override;
        virtual void update(float dt) override;
        virtual void draw(const std::shared_ptr<Interfaces::ICamera> camera) override;

    private:
        void init() override { }

        Objects::Shader m_shader;

        bool m_isInit = false;
    };
}
}
}
}

#endif // GRAPHICLIBRARIES_OPEN_GL_3D_OBJECTS_EXTERNAL_MODEL_HPP_
