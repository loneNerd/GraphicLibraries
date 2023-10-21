#pragma once
#ifndef GRAPHIC_LIBRARIES_OPENGL_OBJECTS_LEVELS_EXTERNAL_MODEL_SHOWCASE_HPP_
#define GRAPHIC_LIBRARIES_OPENGL_OBJECTS_LEVELS_EXTERNAL_MODEL_SHOWCASE_HPP_

#include <memory>

#include "interfaces/level.hpp"

namespace GraphicLibraries
{
namespace OpenGL
{
namespace Objects
{

namespace Models3D { class ExternalModel; }

namespace Levels
{
    class ExternalModelShowcase : public Interfaces::ILevel
    {
    public:
        ExternalModelShowcase();
        virtual ~ExternalModelShowcase();

        ExternalModelShowcase(const ExternalModelShowcase& other)            = delete;
        ExternalModelShowcase(ExternalModelShowcase&& other)                 = delete;
        ExternalModelShowcase& operator=(const ExternalModelShowcase& other) = delete;
        ExternalModelShowcase& operator=(ExternalModelShowcase&& other)      = delete;

        void init(Windows::GLFWWindow* window) override;
        void release() override;
        void updateUI() override;
        void update(float dt) override;
        void draw() override;

    private:
        Windows::GLFWWindow* m_window = nullptr;
        //std::unique_ptr<Models3D::Cube> m_cube = nullptr;
        std::unique_ptr<Models3D::ExternalModel> m_externalModel = nullptr;
        bool m_isInit = false;
    };
}
}
}
}

#endif // GRAPHIC_LIBRARIES_OPENGL_OBJECTS_LEVELS_EXTERNAL_MODEL_SHOWCASE_HPP_
