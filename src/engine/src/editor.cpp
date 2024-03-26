#include "engine/editor.hpp"

#include "windows/sdl2.hpp"

#include "core/renderer.hpp"
#include "opengl/settings/primitive_mode.hpp"
#include "opengl/settings/rendering_capability.hpp"
#include "ui/ui_manager.hpp"
#include "ui/modules/canvas.hpp"
#include "engine/panels/scene_view.hpp"
#include "core/scene_system/scene_manager.hpp"
#include "tools/utils/service_locator.hpp"
#include "windows/inputs/input_manager.hpp"

namespace Core = Engine::Core;

Engine::Editor::Editor(Context& context) : m_context(context)
{
    setupUI();
}

Engine::Editor::~Editor()
{
    if (m_canvas)
        m_canvas = nullptr;

    if (m_sceneView)
        m_sceneView = nullptr;
}

void Engine::Editor::setupUI()
{
    UI::Settings::PanelWindowSettings settings;
    settings.Closable = false;
    settings.Collapsable = false;
    settings.Dockable = false;

    m_canvas = std::make_shared<UI::Modules::Canvas>();

    m_sceneView = std::make_shared<Panels::SceneView>("Scene View", true, settings);

    m_canvas->MakeDockspace(true);
    Tools::Utils::ServiceLocator::Get<UI::UIManager>()->SetCanvas(m_canvas);
}

void Engine::Editor::PreUpdate()
{
    Tools::Utils::ServiceLocator::Get<Windows::SDL2>()->PollEvents();
    Tools::Utils::ServiceLocator::Get<Core::Renderer>()->SetClearColor(0.0f, 0.2f, 0.4f, 1.0f);
    Tools::Utils::ServiceLocator::Get<Core::Renderer>()->Clear();
}

void Engine::Editor::Update(float dt)
{
    std::shared_ptr<Windows::Inputs::InputManager> input = Tools::Utils::ServiceLocator::Get<Windows::Inputs::InputManager>();

    if (input->IsKeyPressed(Engine::Windows::Inputs::EKey::Escape))
        Tools::Utils::ServiceLocator::Get<Windows::SDL2>()->CloseWindow();

    m_sceneView->Update(dt);

    if (m_sceneView->IsOpened())
        m_sceneView->Render();

    //Tools::Utils::ServiceLocator::Get<UI::UIManager>()->Render();
}

void Engine::Editor::PostUpdate()
{
    Tools::Utils::ServiceLocator::Get<Windows::SDL2>()->SwapBuffers();
    ++m_elapsedFrames;
}
