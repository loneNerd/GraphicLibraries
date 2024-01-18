#pragma once
#ifndef ENGINE_CORE_COMPONENTS_SCENE_SYSTEM_SCENE_HPP_
#define ENGINE_CORE_COMPONENTS_SCENE_SYSTEM_SCENE_HPP_

#include "core/actor.hpp"
#include "core/components/model_renderer.hpp"

namespace Engine
{
namespace Core
{
namespace SceneSystem
{
    class Scene
    {
    public:
        Scene() { }
        ~Scene();

        Scene(const Scene& other)             = delete;
        Scene(Scene&& other)                  = delete;
        Scene& operator=(const Scene& other)  = delete;
        Scene& operator=(const Scene&& other) = delete;

        void Update(float dt);
        std::shared_ptr<Actor> CreateActor();
        std::shared_ptr<Actor> CreateActor(const std::string& name, const std::string& tag = "");
        bool DestroyActor(std::shared_ptr<Actor> target);
        void CollectGarbages();
        std::shared_ptr<Actor> FindActorByName(const std::string& name);
        std::shared_ptr<Actor> FindActorByTag(const std::string& tag);
        std::shared_ptr<Actor> FindActorByID(int64_t id);
        std::vector<std::shared_ptr<Actor>> FindActorsByName(const std::string& name);
        std::vector<std::shared_ptr<Actor>> FindActorsByTag(const std::string& tag);
        std::vector<std::shared_ptr<Actor>>& GetActors();

    private:
        int64_t m_availableID = 1;
        std::vector<std::shared_ptr<Actor>> m_actors;
        std::vector<std::shared_ptr<Components::ModelRenderer>> ModelRenderers;
    };
}
}
}

#endif // ENGINE_CORE_COMPONENTS_SCENE_SYSTEM_SCENE_HPP_
