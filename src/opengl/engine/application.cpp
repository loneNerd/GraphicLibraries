#include "application.hpp"

#include "tools/time/clock.hpp"
#include "glfw/glfw.hpp"

namespace Editor = Engine::Editor;

void Editor::Application::Run()
{
    Tools::Time::Clock clock;

    while (IsRunning())
    {
        m_editor.PreUpdate();
        m_editor.Update(clock.GetDeltaTime());
        m_editor.PostUpdate();

        clock.Update();
    }
}

bool Editor::Application::IsRunning() const
{
    return !m_context.Window->ShouldClose();
}
