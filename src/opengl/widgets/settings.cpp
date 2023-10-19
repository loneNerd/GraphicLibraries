#include "settings.hpp"

#include <imgui/imgui.h>

using namespace GraphicLibraries::OpenGL::Widgets;

void Settings::draw()
{
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove;
    ImGui::Begin("Settings", nullptr, windowFlags);
}
