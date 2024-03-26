#include "windows/inputs/input_manager.hpp"

#include <sdl2/SDL.h>

namespace Inputs = Engine::Windows::Inputs;

Inputs::InputManager::InputManager(std::shared_ptr<Windows::SDL2> window) : m_window { window }
{
    m_keyPressedListener = m_window->KeyPressedEvent.AddListener(std::bind(&InputManager::onKeyPressed, this, std::placeholders::_1));
    m_keyReleasedListener = m_window->KeyReleasedEvent.AddListener(std::bind(&InputManager::onKeyReleased, this, std::placeholders::_1));
    m_mouseButtonPressedListener = m_window->MouseButtonPressedEvent.AddListener(std::bind(&InputManager::onMouseButtonPressed, this, std::placeholders::_1));
    m_mouseButtonReleasedListener = m_window->MouseButtonReleasedEvent.AddListener(std::bind(&InputManager::onMouseButtonReleased, this, std::placeholders::_1));
}

Inputs::InputManager::~InputManager()
{
    m_window->KeyPressedEvent.RemoveListener(m_keyPressedListener);
    m_window->KeyReleasedEvent.RemoveListener(m_keyReleasedListener);
    m_window->MouseButtonPressedEvent.RemoveListener(m_mouseButtonPressedListener);
    m_window->MouseButtonReleasedEvent.RemoveListener(m_mouseButtonReleasedListener);
}

//Inputs::EKeyState Inputs::InputManager::GetKeyState(EKey key) const
//{
//    //const Uint8* states = SDL_GetKeyboardState(NULL);
//    ////SDL_PumpEvents();
//    //if (states[static_cast<int>(key)])
//    if (IsKeyPressed(key))
//        return EKeyState::Pressed;
//
//    return EKeyState::Released;
//}

Inputs::EMouseButtonState Inputs::InputManager::GetMouseButtonState(EMouseButton button) const
{
    if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(static_cast<int>(button)))
        return EMouseButtonState::Pressed;

    return EMouseButtonState::Released;
}

bool Inputs::InputManager::IsKeyPressed(EKey key) const
{
    return m_keyEvents.find(key) != m_keyEvents.end() && m_keyEvents.at(key) == EKeyState::Pressed;
}

bool Inputs::InputManager::IsKeyReleased(EKey key) const
{
    return m_keyEvents.find(key) != m_keyEvents.end() && m_keyEvents.at(key) == EKeyState::Released;
}

bool Inputs::InputManager::IsMouseButtonPressed(EMouseButton button) const
{
    return m_mouseButtonEvents.find(button) != m_mouseButtonEvents.end() && m_mouseButtonEvents.at(button) == EMouseButtonState::Pressed;
}

bool Inputs::InputManager::IsMouseButtonReleased(EMouseButton button) const
{
    return m_mouseButtonEvents.find(button) != m_mouseButtonEvents.end() && m_mouseButtonEvents.at(button) == EMouseButtonState::Released;
}

std::pair<int, int> Inputs::InputManager::GetMousePosition() const
{
    std::pair<int, int> result;
    SDL_GetMouseState(&result.first, &result.second);
    return result;
}

void Inputs::InputManager::ClearEvents()
{
    m_keyEvents.clear();
    m_mouseButtonEvents.clear();
}

void Inputs::InputManager::onKeyPressed(int key)
{
    m_keyEvents[static_cast<EKey>(key)] = EKeyState::Pressed;
}

void Inputs::InputManager::onKeyReleased(int key)
{
    m_keyEvents[static_cast<EKey>(key)] = EKeyState::Released;
}

void Inputs::InputManager::onMouseButtonPressed(int button)
{
    m_mouseButtonEvents[static_cast<EMouseButton>(button)] = EMouseButtonState::Pressed;
}

void Inputs::InputManager::onMouseButtonReleased(int button)
{
    m_mouseButtonEvents[static_cast<EMouseButton>(button)] = EMouseButtonState::Released;
}
