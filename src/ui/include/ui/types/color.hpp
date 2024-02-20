#pragma once
#ifndef ENGINE_UI_TYPES_COLOR_HPP_
#define ENGINE_UI_TYPES_COLOR_HPP_

namespace Engine::UI::Types
{
    struct Color
    {
        Color(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f) : R(r), G(g), B(b), A(a) { }

        float R;
        float G;
        float B;
        float A;

        static const Color Red;
        static const Color Green;
        static const Color Blue;
        static const Color White;
        static const Color Black;
        static const Color Grey;
        static const Color Yellow;
        static const Color Cyan;
        static const Color Magenta;

        bool operator==(const Color& other);
        bool operator!=(const Color& other);
    };
}

#endif // ENGINE_UI_TYPES_COLOR_HPP_
