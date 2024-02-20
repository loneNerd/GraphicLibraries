#pragma once
#ifndef ENGINE_OPENGL_RESOURCES_MODEL_HPP_
#define ENGINE_OPENGL_RESOURCES_MODEL_HPP_

#include <filesystem>
#include <list>
#include <memory>
#include <string>

#include "mesh.hpp"

namespace Engine::Core::ResourceManagement { class ModelManager; }
namespace Engine::Core::Resources::Parsers { class AssimpParser; }

namespace Engine::OpenGL::Resources
{
    class Model
    {
    public:
        ~Model();

        Model(const Model& other)             = delete;
        Model(Model&& other)                  = delete;
        Model& operator=(const Model& other)  = delete;
        Model& operator=(const Model&& other) = delete;

        const std::list<std::shared_ptr<Mesh>>& GetMeshes() const;
        const std::filesystem::path Path;

    private:
        friend class Engine::Core::ResourceManagement::ModelManager;
        friend class Engine::Core::Resources::Parsers::AssimpParser;

        Model(std::filesystem::path path) : Path(path) { }

        static std::shared_ptr<Model> createSharedPtr(std::filesystem::path path);
        static std::unique_ptr<Model> createUniquePtr(std::filesystem::path path);

        std::list<std::shared_ptr<Mesh>> m_meshes;
    };
}

#endif // ENGINE_OPENGL_RESOURCES_MODEL_HPP_
