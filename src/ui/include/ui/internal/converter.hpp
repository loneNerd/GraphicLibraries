#pragma once
#ifndef ENGINE_UI_INTERNAL_CONVERTER_HPP_
#define ENGINE_UI_INTERNAL_CONVERTER_HPP_

#include <imgui/imgui.h>

#include "math/vector2.hpp"
#include "ui/types/color.hpp"

namespace Engine::UI::Internal
{
    class Converter
    {
    public:
        Converter() = delete;

        Converter(const Converter& other)             = delete;
        Converter(Converter&& other)                  = delete;
        Converter& operator=(const Converter& other)  = delete;
        Converter& operator=(const Converter&& other) = delete;

        static ImVec4 ToImVec4(const Types::Color& value);
        static Types::Color ToColor(const ImVec4& value);
        static ImVec2 ToImVec2(const Math::FVector2& value);
        static Math::FVector2 ToFVector2(const ImVec2& value);
    };
}

#endif // ENGINE_UI_INTERNAL_CONVERTER_HPP_
