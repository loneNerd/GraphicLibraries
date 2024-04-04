#pragma once
#ifndef ENGINE_WINDOWS_INPUTS_INPUT_MANAGER_HPP_
#define ENGINE_WINDOWS_INPUTS_INPUT_MANAGER_HPP_

#include <unordered_map>

#include "key.hpp"
#include "key_state.hpp"
#include "mouse_button.hpp"
#include "mouse_button_state.hpp"
#include "tools/eventing/event.hpp"
#include "windows/sdl2.hpp"

namespace Engine::Windows::Inputs
{
    class InputManager
    {
    public:
        InputManager(std::shared_ptr<Windows::SDL2> window);
        ~InputManager();

        InputManager(const InputManager& other)             = delete;
        InputManager(InputManager&& other)                  = delete;
        InputManager& operator=(const InputManager& other)  = delete;
        InputManager& operator=(const InputManager&& other) = delete;

        EMouseButtonState GetMouseButtonState(EMouseButton button) const;

        bool IsKeyPressed(EKey key) const;
        bool IsKeyReleased(EKey key) const;
        bool IsMouseButtonPressed(EMouseButton button) const;
        bool IsMouseButtonReleased(EMouseButton button) const;

        std::pair<int, int> GetMousePosition() const;

        void ClearEvents();

    private:
        void onKeyPressed(int key);
        void onKeyReleased(int key);
        void onMouseButtonPressed(int button);
        void onMouseButtonReleased(int button);

        std::shared_ptr<Windows::SDL2> m_window;

        Tools::Eventing::ListenerID m_keyPressedListener;
        Tools::Eventing::ListenerID m_keyReleasedListener;
        Tools::Eventing::ListenerID m_mouseButtonPressedListener;
        Tools::Eventing::ListenerID m_mouseButtonReleasedListener;

        std::unordered_map<EKey, EKeyState>                 m_keyEvents;
        std::unordered_map<EMouseButton, EMouseButtonState>	m_mouseButtonEvents;
    };
}

#endif // ENGINE_WINDOWS_INPUTS_INPUT_MANAGER_HPP_
