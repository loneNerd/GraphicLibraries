#pragma once
#ifndef ENGINE_UI_MODULES_CANVAS_HPP_
#define ENGINE_UI_MODULES_CANVAS_HPP_

#include <memory>
#include <list>

#include "ui/drawable_interface.hpp"
//#include "ui/panels/base_panel.hpp"

namespace Engine::UI::Panels { class BasePanel; }

namespace Engine::UI::Modules
{
    class Canvas : public IDrawable
    {
    public:
        Canvas()  = default;

        Canvas(const Canvas& other)             = delete;
        Canvas(Canvas&& other)                  = delete;
        Canvas& operator=(const Canvas& other)  = delete;
        Canvas& operator=(const Canvas&& other) = delete;

        //void AddPanel(Panels::BasePanel& panel);
        //void RemovePanel(Panels::BasePanel& panel);
        template<typename T, typename... Args>
        std::shared_ptr<T> AddPanel(Args&&... args);

        //template<typename T>
        //bool RemovePanel();

        void RemovePanel(std::shared_ptr<Panels::BasePanel> panel);

        void RemoveAllPanels();
        void MakeDockspace(bool state);
        bool IsDockspace() const;
        void Draw() override;

    private:
        std::list<std::shared_ptr<Panels::BasePanel>> m_panels;
        bool m_isDockspace = false;
    };
}

#include "ui/modules/canvas.inl"

#endif // ENGINE_UI_MODULES_CANVAS_HPP_
