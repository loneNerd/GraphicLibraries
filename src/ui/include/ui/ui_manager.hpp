#pragma once
#ifndef ENGINE_UI_UI_MANAGER_HPP_
#define ENGINE_UI_UI_MANAGER_HPP_

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

#include <imgui/imgui.h>

//#include "ui/modules/canvas.hpp"
#include "styling/style.hpp"

namespace Engine::Windows { class SDL2; }

namespace Engine::UI
{
    namespace Modules { class Canvas; }

    class UIManager
    {
    public:
        UIManager(const std::shared_ptr<Windows::SDL2> window, Styling::EStyle style = Styling::EStyle::ImDarkStyle, const std::string& glslVersion = "#version 150");
        ~UIManager();

        UIManager(const UIManager& other)             = delete;
        UIManager(UIManager&& other)                  = delete;
        UIManager& operator=(const UIManager& other)  = delete;
        UIManager& operator=(const UIManager&& other) = delete;

        void ApplyStyle(Styling::EStyle style);
        void SetEditorLayoutAutosaveFrequency(float frequency);
        float GetEditorLayoutAutosaveFrequency();
        void EnableDocking(bool value);
        bool IsDockingEnabled() const;
        void SetCanvas(std::shared_ptr<Modules::Canvas> canvas);
        void RemoveCanvas();
        void Render();

    private:
        bool m_dockingState = false;
        std::shared_ptr<Modules::Canvas> m_currentCanvas = nullptr;
    };
}

#endif // ENGINE_UI_UI_MANAGER_HPP_
