#pragma once
#ifndef ENGINE_MATH_MATRIX4_HPP_
#define ENGINE_MATH_MATRIX4_HPP_

#include <array>
#include <stdint.h>

#include "vector3.hpp"
#include "vector4.hpp"

namespace Engine
{
namespace Math
{
    struct FQuaternion;

    struct FMatrix4
    {
        std::array<float, 16> Data;
        static const FMatrix4 Identity;

        FMatrix4();

        FMatrix4(float element1,  float element2,  float element3,
                 float element4,  float element5,  float element6,
                 float element7,  float element8,  float element9,
                 float element10, float element11, float element12,
                 float element13, float element14, float element15, float element16);

        FMatrix4(const FMatrix4& other);

        FMatrix4& operator=(const FMatrix4& other);
        bool operator==(const FMatrix4& other);
        FMatrix4 operator+(const FMatrix4& other) const;
        FMatrix4& operator+=(const FMatrix4& other);
        FMatrix4 operator-(float scalar) const;
        FMatrix4 operator-(const FMatrix4& other) const;
        FMatrix4& operator-=(const FMatrix4& other);
        FMatrix4 operator*(float scalar) const;
        FMatrix4& operator*=(float scalar);
        FVector4 operator*(const FVector4& vector) const;
        FMatrix4 operator*(const FMatrix4& other) const;
        FMatrix4& operator*=(const FMatrix4& other);
        FMatrix4 operator/(float scalar) const;
        FMatrix4& operator/=(float scalar);
        FMatrix4 operator/(const FMatrix4& other) const;
        FMatrix4& operator/=(const FMatrix4& other);
        float& operator()(uint8_t row, uint8_t column);

        static bool AreEquals(const FMatrix4& left, const FMatrix4& right);
        static FMatrix4 Add(const FMatrix4& left, float scalar);
        static FMatrix4 Add(const FMatrix4& left, const FMatrix4& right);
        static FMatrix4 Subtract(const FMatrix4& left, float scalar);
        static FMatrix4 Subtract(const FMatrix4& left, const FMatrix4& right);
        static FMatrix4 Multiply(const FMatrix4& left, float scalar);
        static FVector4 Multiply(const FMatrix4& matrix, const FVector4& vector);
        static FMatrix4 Multiply(const FMatrix4& left, const FMatrix4& right);
        static FMatrix4 Divide(const FMatrix4& left, float scalar);
        static FMatrix4 Divide(const FMatrix4& left, const FMatrix4& right);
        static bool IsIdentity(const FMatrix4& matrix);

        static float GetMinor(float minor0, float minor1, float minor2,
                              float minor3, float minor4, float minor5,
                              float minor6, float minor7, float minor8);

        static float Determinant(const FMatrix4& matrix);
        static FMatrix4 Transpose(const FMatrix4& matrix);
        static FMatrix4 Inverse(const FMatrix4& matrix);
        static FMatrix4 Translation(const FVector3& translation);
        static FMatrix4 Translate(const FMatrix4& matrix, const FVector3& translation);
        static FMatrix4 RotationOnAxisX(float rotation);
        static FMatrix4 RotateOnAxisX(const FMatrix4& matrix, float rotation);
        static FMatrix4 RotationOnAxisY(float rotation);
        static FMatrix4 RotateOnAxisY(const FMatrix4& matrix, float rotation);
        static FMatrix4 RotationOnAxisZ(float rotation);
        static FMatrix4 RotateOnAxisZ(const FMatrix4& matrix, float rotation);
        static FMatrix4 RotationYXZ(float rotation);
        static FMatrix4 RotateYXZ(const FMatrix4& matrix, float rotation);
        static FMatrix4 Scaling(const FVector3& scale);
        static FMatrix4 Scale(const FMatrix4& matrix, const FVector3& scale);
        static FMatrix4 Rotation(const FQuaternion& quaternion);
        static FMatrix4 Rotate(const FMatrix4& matrix, const FQuaternion& quaternion);
        static FMatrix4 CreatePerspective(const float fov, const float aspectRatio, const float zNear, const float zFar);
        static FMatrix4 CreateOrthographic(const float size, const float aspectRatio, const float zNear, const float zFar);
        static FMatrix4 CreateView(const float eyeX, const float eyeY, const float eyeZ, const float lookX, const float lookY, const float lookZ, const float upX, const float upY, const float upZ);
        static FMatrix4 CreateFrustum(const float left, const float right, const float bottom, const float top, const float zNear, const float zFar);
        static FVector4 GetRow(const FMatrix4& matrix, uint8_t row);
        static FVector4 GetColumn(const FMatrix4& matrix, uint8_t column);
    };
}
}

#endif // ENGINE_MATH_MATRIX4_HPP_
