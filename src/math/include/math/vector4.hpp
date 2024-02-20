#pragma once
#ifndef ENGINE_MATH_VECTOR4_HPP_
#define ENGINE_MATH_VECTOR4_HPP_

namespace Engine::Math
{
    struct FVector4
    {
        static const FVector4 One;
        static const FVector4 Zero;

        float X;
        float Y;
        float Z;
        float W;

        FVector4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) : X(x), Y(y), Z(z), W(w) { }
        FVector4(const FVector4& toCopy) : X(toCopy.X), Y(toCopy.Y), Z(toCopy.Z), W(toCopy.W) { }
        FVector4(FVector4&& toMove) noexcept = default;

        FVector4 operator-() const;
        FVector4 operator=(const FVector4& other);
        FVector4 operator+(const FVector4& other) const;
        FVector4& operator+=(const FVector4& other);
        FVector4 operator-(const FVector4& other) const;
        FVector4& operator-=(const FVector4& other);
        FVector4 operator*(float scalar) const;
        FVector4& operator*=(float scalar);
        FVector4 operator/(float scalar) const;
        FVector4& operator/=(float scalar);
        bool operator==(const FVector4& other);
        bool operator!=(const FVector4& other);

        static FVector4 Add(const FVector4& left, const FVector4& right);
        static FVector4 Substract(const FVector4& left, const FVector4& right);
        static FVector4 Multiply(const FVector4& target, float scalar);
        static FVector4 Divide(const FVector4& left, float scalar);
        static float Length(const FVector4& target);
        static float Dot(const FVector4& left, const FVector4& right);
        static FVector4 Normalize(const FVector4& target);
        static FVector4 Lerp(const FVector4& start, const FVector4& end, float alpha);
    };
}

#endif // ENGINE_MATH_VECTOR4_HPP_
