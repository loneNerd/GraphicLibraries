#include "ui/modules/canvas.hpp"
#include "ui/panels/base_panel.hpp"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_opengl3.h>

namespace UI = Engine::UI;

//void UI::Modules::Canvas::AddPanel(Panels::IPanel& panel)
//{
//    m_panels.push_back(std::ref(panel));
//}
//
//void UI::Modules::Canvas::RemovePanel(Panels::IPanel& panel)
//{
//    m_panels.erase(std::remove_if(m_panels.begin(), m_panels.end(), [&panel](std::reference_wrapper<Panels::IPanel>& item)
//        {
//            return &panel == &item.get();
//        }));
//}

void UI::Modules::Canvas::RemovePanel(std::shared_ptr<Panels::BasePanel> panel)
{
    m_panels.erase(std::remove_if(m_panels.begin(), m_panels.end(), [&panel](std::shared_ptr<Panels::BasePanel>& item)
        {
            return panel == item;
        }));
}

void UI::Modules::Canvas::RemoveAllPanels()
{
    m_panels.clear();
}

void UI::Modules::Canvas::MakeDockspace(bool state)
{
    m_isDockspace = state;
}

bool UI::Modules::Canvas::IsDockspace() const
{
    return m_isDockspace;
}

void UI::Modules::Canvas::Draw()
{
    if (!m_panels.empty())
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        if (m_isDockspace)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

            ImGui::Begin("##dockspace", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking);
            ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
            ImGui::SetWindowPos({ 0.0f, 0.0f });
            ImVec2 displaySize = ImGui::GetIO().DisplaySize;
            ImGui::SetWindowSize({ static_cast<float>(displaySize.x), static_cast<float>(displaySize.y) });
            ImGui::End();

            ImGui::PopStyleVar(3);
        }

        for (auto& panel : m_panels)
            panel->Draw();

        ImGui::Render();
    }
}
