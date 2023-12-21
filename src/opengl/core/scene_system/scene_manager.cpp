#include "scene_manager.hpp"

namespace SceneSystem = Engine::Core::SceneSystem;
namespace FS = std::filesystem;

SceneSystem::SceneManager::SceneManager(const FS::path& sceneRootFolder) : m_sceneRootFolder(sceneRootFolder)
{
    LoadEmptyScene();
}

SceneSystem::SceneManager::~SceneManager()
{
    UnloadCurrentScene();
}

void SceneSystem::SceneManager::Update()
{
}

void SceneSystem::SceneManager::LoadEmptyScene()
{
    UnloadCurrentScene();
    m_currentScene = std::make_shared<Scene>();
}

void SceneSystem::SceneManager::UnloadCurrentScene()
{
    if (m_currentScene)
    {
        m_currentScene = nullptr;
    }

    ForgetCurrentSceneSourcePath();
}

bool SceneSystem::SceneManager::HasCurrentScene() const
{
    return m_currentScene != nullptr;
}

std::shared_ptr<SceneSystem::Scene> SceneSystem::SceneManager::GetCurrentScene()
{
    return m_currentScene;
}

FS::path SceneSystem::SceneManager::GetCurrentSceneSourcePath() const
{
    return m_currentSceneSourcePath;
}

bool SceneSystem::SceneManager::IsCurrentSceneLoadedFromDisk() const
{
    return m_currentSceneLoadedFromPath;
}

void SceneSystem::SceneManager::SetCurrentSceneSourcePath(const FS::path& path)
{
    m_currentSceneSourcePath = path;
    m_currentSceneLoadedFromPath = true;
}

void SceneSystem::SceneManager::ForgetCurrentSceneSourcePath()
{
    m_currentSceneSourcePath = "";
    m_currentSceneLoadedFromPath = false;
}
