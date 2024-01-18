#pragma once
#ifndef ENGINE_MATH_VECTOR3_HPP_
#define ENGINE_MATH_VECTOR3_HPP_

namespace Engine
{
namespace Math
{
    struct FVector3
    {
        //static const FVector3 One;
        //static const FVector3 Zero;
        //static const FVector3 Forward;
        //static const FVector3 Right;
        //static const FVector3 Up;

        float X;
        float Y;
        float Z;

        FVector3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : X(x), Y(y), Z(z) { }
        FVector3(const FVector3& toCopy) : X(toCopy.X), Y(toCopy.Y), Z(toCopy.Z) { }
        FVector3(FVector3&& toMove) noexcept = default;

        FVector3 operator-() const;
        FVector3 operator=(const FVector3& other);
        FVector3 operator+(const FVector3& other) const;
        FVector3& operator+=(const FVector3& other);
        FVector3 operator-(const FVector3& other) const;
        FVector3& operator-=(const FVector3& other);
        FVector3 operator*(float scalar) const;
        FVector3& operator*=(float scalar);
        FVector3 operator/(float scalar) const;
        FVector3& operator/=(float scalar);
        bool operator==(const FVector3& other);
        bool operator!=(const FVector3& other);

        static FVector3 Add(const FVector3& left, const FVector3& right);
        static FVector3 Substract(const FVector3& left, const FVector3& right);
        static FVector3 Multiply(const FVector3& target, float scalar);
        static FVector3 Divide(const FVector3& left, float scalar);
        static float Length(const FVector3& target);
        static float Dot(const FVector3& left, const FVector3& right);
        static float Distance(const FVector3& left, const FVector3& right);
        static FVector3 Cross(const FVector3& left, const FVector3& right);
        static FVector3 Normalize(const FVector3& target);
        static FVector3 Lerp(const FVector3& start, const FVector3& end, float alpha);
        static float AngleBetween(const FVector3& from, const FVector3& to);
    };
}
}

#endif // ENGINE_MATH_VECTOR3_HPP_
