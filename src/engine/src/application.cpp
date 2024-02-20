#include "engine/application.hpp"

#include "tools/time/clock.hpp"
#include "tools/utils/service_locator.hpp"
#include "windows/sdl2.hpp"

void Engine::Application::Run()
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

bool Engine::Application::IsRunning() const
{
    return !Tools::Utils::ServiceLocator::Get<Windows::SDL2>()->ShouldClose();
}
