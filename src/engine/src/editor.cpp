#include "engine/editor.hpp"

#include "windows/sdl2.hpp"

#include "core/renderer.hpp"
#include "opengl/settings/primitive_mode.hpp"
#include "opengl/settings/rendering_capability.hpp"
#include "ui/ui_manager.hpp"
#include "engine/panels/scene_view.hpp"
#include "core/scene_system/scene_manager.hpp"
#include "tools/utils/service_locator.hpp"

namespace Core = Engine::Core;

Engine::Editor::Editor(Context& context) : m_context(context)
{
    setupUI();
}

Engine::Editor::~Editor()
{
    if (m_sceneView)
        m_sceneView = nullptr;
}

void Engine::Editor::setupUI()
{
    UI::Settings::PanelWindowSettings settings;
    settings.Closable = false;
    settings.Collapsable = false;
    settings.Dockable = false;

    m_sceneView = std::make_shared<Panels::SceneView>("Scene View", true, settings);
}

void Engine::Editor::PreUpdate()
{
    Tools::Utils::ServiceLocator::Get<Windows::SDL2>()->PollEvents();
    Tools::Utils::ServiceLocator::Get<Core::Renderer>()->SetClearColor(0.0f, 0.2f, 0.4f, 1.0f);
    Tools::Utils::ServiceLocator::Get<Core::Renderer>()->Clear();
}

void Engine::Editor::Update(float dt)
{
    m_sceneView->Render();
}

void Engine::Editor::PostUpdate()
{
    Tools::Utils::ServiceLocator::Get<Windows::SDL2>()->SwapBuffers();
    ++m_elapsedFrames;
}
