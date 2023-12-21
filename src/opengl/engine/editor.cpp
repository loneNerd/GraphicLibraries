#include "editor.hpp"

#include "glfw/device.hpp"
#include "glfw/glfw.hpp"

#include "core/renderer.hpp"
#include "core/components/material_renderer.hpp"
#include "opengl/camera.hpp"
#include "opengl/resources/shader.hpp"
#include "opengl/settings/primitive_mode.hpp"
#include "opengl/settings/rendering_capability.hpp"

namespace EditorNS = Engine::Editor;
namespace ResManagement = Engine::Core::ResourceManagement;
namespace Parsers = Engine::OpenGL::Resources::Parsers;
namespace OGL = Engine::OpenGL;
namespace OGLResources = OGL::Resources;
namespace CoreResources = Engine::Core::Resources;
namespace Components = Engine::Core::Components;

EditorNS::Editor::Editor(Context& context) : m_context(context)
{
    ResManagement::ShaderManager& ShaderManagerRef = ResManagement::ShaderManager::Instance();
    ResManagement::ModelManager& ModelManagerRef = ResManagement::ModelManager::Instance();
    ResManagement::MaterialManager& MaterialManagerRef = ResManagement::MaterialManager::Instance();

    m_camera = std::make_shared<OGL::Camera>();

    m_camera->SetProjectionMode(OGL::Settings::EProjectionMode::Perspective);
    std::pair<uint16_t, uint16_t> size = m_context.Window->GetSize();
    m_camera->CacheMatrices(size.first, size.second, m_cameraPosition, m_cameraRotation);

    m_context.SceneManager.LoadEmptyScene();

    const std::shared_ptr<Core::SceneSystem::Scene> currentScene = m_context.SceneManager.GetCurrentScene();
    std::shared_ptr<Core::Actor> instance = currentScene->CreateActor("Cube");

    std::shared_ptr<Core::Components::ModelRenderer> modelRenderer = instance->AddComponent<Core::Components::ModelRenderer>();

    Parsers::EModelParserFlags parsers = Parsers::EModelParserFlags::CalcTangentSpace      |
                                         Parsers::EModelParserFlags::JoinIdenticalVertices |
                                         Parsers::EModelParserFlags::Triangulate           |
                                         Parsers::EModelParserFlags::GenSmoothNormals      |
                                         Parsers::EModelParserFlags::FlipUVs               |
                                         Parsers::EModelParserFlags::ImproveCacheLocality  |
                                         Parsers::EModelParserFlags::FindInvalidData       |
                                         Parsers::EModelParserFlags::GenUVCoords           |
                                         Parsers::EModelParserFlags::FindInstances         |
                                         Parsers::EModelParserFlags::OptimizeMeshes        |
                                         Parsers::EModelParserFlags::OptimizeGraph         |
                                         Parsers::EModelParserFlags::Debone                |
                                         Parsers::EModelParserFlags::GlobalScale;

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

EditorNS::Editor::~Editor()
{
    std::shared_ptr<Core::Actor> model = m_context.SceneManager.GetCurrentScene()->FindActorByName("Cube");

    m_context.SceneManager.UnloadCurrentScene();

    ResManagement::ShaderManager::Instance().Destroy(m_shader->Name);
    ResManagement::ModelManager::Instance().Destroy("models\\cube\\cube.fbx");

    if (m_camera)
        m_camera = nullptr;
}

void EditorNS::Editor::PreUpdate()
{
    m_context.Device->PollEvents();
    m_context.Renderer->SetClearColor(0.0f, 0.2f, 0.4f, 1.0f);
    m_context.Renderer->Clear();
}

void EditorNS::Editor::Update(float dt)
{
    m_context.Renderer->RenderScene(m_context.SceneManager.GetCurrentScene(), m_camera, m_cameraPosition, m_cameraRotation, m_shader);
}

void EditorNS::Editor::PostUpdate()
{
    m_context.Window->SwapBuffers();
    ++m_elapsedFrames;
}
