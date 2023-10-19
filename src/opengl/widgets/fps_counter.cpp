#include "fps_counter.hpp"

#include <imgui/imgui.h>

using namespace GraphicLibraries::OpenGL::Widgets;

void FpsCounter::draw()
{
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration       |
                                   ImGuiWindowFlags_AlwaysAutoResize   |
                                   ImGuiWindowFlags_NoSavedSettings    |
                                   ImGuiWindowFlags_NoFocusOnAppearing |
                                   ImGuiWindowFlags_NoNav              |
                                   ImGuiWindowFlags_NoMove;

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always, ImVec2(0, 0));

    ImGui::Begin("FPS Counter", nullptr, windowFlags);
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::End();
}
