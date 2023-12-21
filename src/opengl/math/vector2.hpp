#pragma once
#ifndef ENGINE_MATH_VECTOR2_HPP_
#define ENGINE_MATH_VECTOR2_HPP_

namespace Engine
{
namespace Math
{
    struct FVector2
    {
        static const FVector2 One;
        static const FVector2 Zero;

        float X;
        float Y;

        FVector2(float x = 0.0f, float y = 0.0f) : X(x), Y(y) { }
        FVector2(const FVector2& toCopy) : X(toCopy.X), Y(toCopy.Y) { }
        FVector2(FVector2&& toMove) noexcept = default;


        FVector2 operator-() const;
        FVector2 operator=(const FVector2& other);
        FVector2 operator+(const FVector2& other) const;
        FVector2& operator+=(const FVector2& other);
        FVector2 operator-(const FVector2& other) const;
        FVector2& operator-=(const FVector2& other);
        FVector2 operator*(float scalar) const;
        FVector2& operator*=(float scalar);
        FVector2 operator/(float scalar) const;
        FVector2& operator/=(float scalar);
        bool operator==(const FVector2& other);
        bool operator!=(const FVector2& other);

        static FVector2 Add(const FVector2& left, const FVector2& right);
        static FVector2 Substract(const FVector2& left, const FVector2& right);
        static FVector2 Multiply(const FVector2& target, float scalar);
        static FVector2 Divide(const FVector2& left, float scalar);
        static float Length(const FVector2& target);
        static float Dot(const FVector2& left, const FVector2& right);
        static FVector2 Normalize(const FVector2& target);
        static FVector2 Lerp(const FVector2& start, const FVector2& end, float alpha);
        static float AngleBetween(const FVector2& from, const FVector2& to);
    };
}
}

#endif // ENGINE_MATH_VECTOR2_HPP_
