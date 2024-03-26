#include "engine/panels/scene_view.hpp"

#include <imgui/imgui.h>

#include "core/components/material_renderer.hpp"
#include "core/renderer.hpp"
#include "core/resource_management/material_manager.hpp"
#include "core/resource_management/model_manager.hpp"
#include "core/resource_management/shader_manager.hpp"
#include "core/scene_system/scene_manager.hpp"
#include "opengl/camera.hpp"
#include "opengl/resources/shader.hpp"
#include "tools/utils/service_locator.hpp"
#include "windows/sdl2.hpp"

namespace Core = Engine::Core;
namespace OpenGL = Engine::OpenGL;
namespace Panels = Engine::Panels;
namespace Parsers = Engine::Core::Resources::Parsers;
namespace ResManagement = Engine::Core::ResourceManagement;
namespace Windows = Engine::Windows;

Panels::SceneView::SceneView
(
    const std::string& title,
    bool opened,
    const UI::Settings::PanelWindowSettings& windowSettings
) : ControllableView(title, opened, windowSettings, true)
{
    m_camera->SetFar(5000.0f);
    m_camera->SetProjectionMode(OpenGL::Settings::EProjectionMode::Perspective);
    std::pair<uint16_t, uint16_t> size = Tools::Utils::ServiceLocator::Get<Windows::SDL2>()->GetSize();
    m_camera->CacheMatrices(size.first, size.second, m_cameraPosition, m_cameraRotation);

    ResManagement::ShaderManager& ShaderManagerRef = ResManagement::ShaderManager::Instance();
    ResManagement::ModelManager& ModelManagerRef = ResManagement::ModelManager::Instance();
    ResManagement::MaterialManager& MaterialManagerRef = ResManagement::MaterialManager::Instance();

    std::shared_ptr<Core::SceneSystem::SceneManager> sceneManager = Tools::Utils::ServiceLocator::Get<Core::SceneSystem::SceneManager>();
    sceneManager->LoadEmptyScene();

    const std::shared_ptr<Core::SceneSystem::Scene> currentScene = sceneManager->GetCurrentScene();
    std::shared_ptr<Core::Actor> instance = currentScene->CreateActor("Cube");

    std::shared_ptr<Core::Components::ModelRenderer> modelRenderer = instance->AddComponent<Core::Components::ModelRenderer>();

    Parsers::EAssimpParserFlags parsers = Parsers::EAssimpParserFlags::CalcTangentSpace |
                                          Parsers::EAssimpParserFlags::JoinIdenticalVertices |
                                          Parsers::EAssimpParserFlags::Triangulate |
                                          Parsers::EAssimpParserFlags::GenSmoothNormals |
                                          Parsers::EAssimpParserFlags::FlipUVs |
                                          Parsers::EAssimpParserFlags::ImproveCacheLocality |
                                          Parsers::EAssimpParserFlags::FindInvalidData |
                                          Parsers::EAssimpParserFlags::GenUVCoords |
                                          Parsers::EAssimpParserFlags::FindInstances |
                                          Parsers::EAssimpParserFlags::OptimizeMeshes |
                                          Parsers::EAssimpParserFlags::OptimizeGraph |
                                          Parsers::EAssimpParserFlags::Debone |
                                          Parsers::EAssimpParserFlags::GlobalScale;

    const std::shared_ptr<OpenGL::Resources::Model> model = ModelManagerRef.Create(ModelManagerRef.GetRealPath("models\\cube\\cube.fbx"), parsers);
    if (model)
        modelRenderer->SetModel(model);

    std::shared_ptr<Core::Components::MaterialRenderer> materialRenderer = instance->AddComponent<Core::Components::MaterialRenderer>();
    const std::shared_ptr<Core::Resources::Material> material = MaterialManagerRef[model->GetMeshes().begin()->get()->GetMaterialName()];

    m_shader = ShaderManagerRef.Create(
        ShaderManagerRef.GetRealPath("3d_external_model_vertex.glsl"),
        ShaderManagerRef.GetRealPath("3d_external_model_fragment.glsl"),
        std::nullopt);

    if (material)
        materialRenderer->FillWithMaterial(material);
}

Panels::SceneView::~SceneView()
{
    std::shared_ptr<Core::SceneSystem::SceneManager> sceneManager = Tools::Utils::ServiceLocator::Get<Core::SceneSystem::SceneManager>();

    std::shared_ptr<Core::Actor> model = sceneManager->GetCurrentScene()->FindActorByName("Cube");

    sceneManager->UnloadCurrentScene();

    ResManagement::ShaderManager::Instance().Destroy(m_shader->Name);
    ResManagement::ModelManager::Instance().Destroy("models\\cube\\cube.fbx");
}

void Panels::SceneView::RenderImpl()
{
    prepareCamera();
    std::shared_ptr<Core::Renderer> renderer = Tools::Utils::ServiceLocator::Get<Core::Renderer>();
    renderer->RenderScene(Tools::Utils::ServiceLocator::Get<Core::SceneSystem::SceneManager>()->GetCurrentScene(), m_camera, m_cameraPosition, m_cameraRotation, m_shader);
}
