#include "ui/internal/converter.hpp"

namespace Internal = Engine::UI::Internal;
namespace Math = Engine::Math;
namespace Types = Engine::UI::Types;

ImVec4 Internal::Converter::ToImVec4(const Types::Color& value)
{
    return ImVec4(value.R, value.G, value.B, value.A);
}

Types::Color Internal::Converter::ToColor(const ImVec4& value)
{
    return Types::Color(value.x, value.y, value.z, value.w);
}

ImVec2 Internal::Converter::ToImVec2(const Math::FVector2& value)
{
    return ImVec2(value.X, value.Y);
}

Math::FVector2 Internal::Converter::ToFVector2(const ImVec2& value)
{
    return Math::FVector2(value.x, value.y);
}
