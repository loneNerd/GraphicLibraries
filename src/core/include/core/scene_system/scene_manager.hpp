#pragma once
#ifndef ENGINE_CORE_COMPONENTS_SCENE_SYSTEM_SCENE_MANAGER_HPP_
#define ENGINE_CORE_COMPONENTS_SCENE_SYSTEM_SCENE_MANAGER_HPP_

#include <filesystem>

#include "scene.hpp"

namespace Engine::Core::SceneSystem
{
    class SceneManager
    {
    public:
        SceneManager(const std::filesystem::path& sceneRootFolder = "");
        ~SceneManager();

        SceneManager(const SceneManager& other)             = delete;
        SceneManager(SceneManager&& other)                  = delete;
        SceneManager& operator=(const SceneManager& other)  = delete;
        SceneManager& operator=(const SceneManager&& other) = delete;

        void LoadEmptyScene();
        void UnloadCurrentScene();
        bool HasCurrentScene() const;
        std::shared_ptr<Scene> GetCurrentScene();
        std::filesystem::path GetCurrentSceneSourcePath() const;
        bool IsCurrentSceneLoadedFromDisk() const;
        void SetCurrentSceneSourcePath(const std::filesystem::path& path);
        void ForgetCurrentSceneSourcePath();

    private:
        const std::filesystem::path m_sceneRootFolder;
        std::shared_ptr<Scene> m_currentScene = nullptr;

        bool m_currentSceneLoadedFromPath = false;
        std::filesystem::path m_currentSceneSourcePath = "";
    };
}

#endif // ENGINE_CORE_COMPONENTS_SCENE_SYSTEM_SCENE_MANAGER_HPP_
