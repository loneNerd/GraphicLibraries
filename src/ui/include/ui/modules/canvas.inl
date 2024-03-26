#pragma once
#ifndef ENGINE_UI_MODULES_CANVAS_INL_
#define ENGINE_UI_MODULES_CANVAS_INL_

#include "ui/modules/canvas.hpp"

namespace UI = Engine::UI;

template<typename T, typename... Args>
std::shared_ptr<T> UI::Modules::Canvas::AddPanel(Args&&... args)
{
    static_assert(std::is_base_of<Panels::BasePanel, T>::value, "T should derive from BasePanel");

    std::shared_ptr<T> instance = std::make_shared<T>(args...);
    m_components.emplace(m_components.begin(), instance);
    return instance;
}

#endif // ENGINE_UI_MODULES_CANVAS_INL_
