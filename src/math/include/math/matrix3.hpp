#pragma once
#ifndef ENGINE_MATH_MATRIX3_HPP_
#define ENGINE_MATH_MATRIX3_HPP_

#include <array>
#include <stdint.h>

#include "vector2.hpp"
#include "vector3.hpp"

namespace Engine
{
namespace Math
{
    struct FMatrix3
    {
        std::array<float, 9> Data = { 0.0f };

        static const FMatrix3 Identity;

        FMatrix3();
        FMatrix3(float value);

        FMatrix3(float element1, float element2, float element3,
                 float element4, float element5, float element6,
                 float element7, float element8, float element9);

        FMatrix3(const FMatrix3& other);

        FMatrix3& operator=(const FMatrix3& other);
        bool operator==(const FMatrix3& other);
        FMatrix3 operator+(const FMatrix3& other) const;
        FMatrix3& operator+=(const FMatrix3& other);
        FMatrix3 operator-(const FMatrix3& other) const;
        FMatrix3& operator-=(const FMatrix3& other);
        FMatrix3 operator*(float scalar) const;
        FMatrix3& operator*=(float scalar);
        FVector3 operator*(const FVector3& vector) const;
        FMatrix3 operator*(const FMatrix3& other) const;
        FMatrix3& operator*=(const FMatrix3& other);
        FMatrix3 operator/(float scalar) const;
        FMatrix3& operator/=(float scalar);
        FMatrix3 operator/(const FMatrix3& other) const;
        FMatrix3& operator/=(const FMatrix3& other);
        float& operator()(uint8_t row, uint8_t column);

        static bool AreEquals(const FMatrix3& left, const FMatrix3& right);
        static FMatrix3 Add(const FMatrix3& left, float scalar);
        static FMatrix3 Add(const FMatrix3& left, const FMatrix3& right);
        static FMatrix3 Subtract(const FMatrix3& left, float scalar);
        static FMatrix3 Subtract(const FMatrix3& left, const FMatrix3& right);
        static FMatrix3 Multiply(const FMatrix3& left, float scalar);
        static FVector3 Multiply(const FMatrix3& matrix, const FVector3& vector);
        static FMatrix3 Multiply(const FMatrix3& left, const FMatrix3& right);
        static FMatrix3 Divide(const FMatrix3& left, float scalar);
        static FMatrix3 Divide(const FMatrix3& left, const FMatrix3& right);
        static bool IsIdentity(const FMatrix3& matrix);
        static float Determinant(const FMatrix3& matrix);
        static FMatrix3 Transpose(const FMatrix3& matrix);
        static FMatrix3 Cofactor(const FMatrix3& matrix);
        static FMatrix3 Minor(const FMatrix3& matrix);
        static FMatrix3 Adjoint(const FMatrix3& other);
        static FMatrix3 Inverse(const FMatrix3& matrix);
        static FMatrix3 Translation(const FVector2& translation);
        static FMatrix3 Translate(const FMatrix3& matrix, const FVector2& translation);
        static FMatrix3 Rotation(float rotation);
        static FMatrix3 Rotate(const FMatrix3& matrix, float rotation);
        static FMatrix3 Scaling(const FVector2& scale);
        static FMatrix3 Scale(const FMatrix3& matrix, const FVector2& scale);
        static FVector3 GetRow(const FMatrix3& matrix, uint8_t row);
        static FVector3 GetColumn(const FMatrix3& matrix, uint8_t column);
    };
}
}

#endif // ENGINE_MATH_MATRIX3_HPP_
