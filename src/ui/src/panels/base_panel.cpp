#include "ui/panels/base_panel.hpp"

#include <algorithm>

namespace Panels = Engine::UI::Panels;

uint64_t Panels::BasePanel::ID = 0;

Panels::BasePanel::BasePanel()
{
    m_panelID = "##" + std::to_string(ID++);
}

void Panels::BasePanel::Draw()
{
    if (m_enabled)
        DrawImpl();
}

const std::string& Panels::BasePanel::GetPanelID() const
{
    return m_panelID;
}
