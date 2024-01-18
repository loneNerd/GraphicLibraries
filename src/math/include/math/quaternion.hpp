#pragma once
#ifndef ENGINE_MATH_QUATERNION_HPP_
#define ENGINE_MATH_QUATERNION_HPP_

#include <string>

#include "matrix3.hpp"
#include "matrix4.hpp"
#include "vector3.hpp"

namespace Engine
{
namespace Math
{
    struct FQuaternion
    {
    public:
        float X;
        float Y;
        float Z;
        float W;

        //static const FQuaternion Identity;

        FQuaternion() : X(0.0f), Y(0.0f), Z(0.0f), W(1.0f) { }
        FQuaternion(float real) : X(0.0f), Y(0.0f), Z(0.0f), W(real) { }
        FQuaternion(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) { }
        FQuaternion(const FQuaternion& other) : X(other.X), Y(other.Y), Z(other.Z), W(other.W) { }
        FQuaternion(const FMatrix3& rotationMatrix);
        FQuaternion(const FMatrix4& rotationMatrix);
        FQuaternion(const FVector3& euler);

        bool operator==(const FQuaternion& otherQuat) const;
        bool operator!=(const FQuaternion& otherQuat) const;
        FQuaternion operator+(const FQuaternion& otherQuat) const;
        FQuaternion& operator+=(const FQuaternion& otherQuat);
        FQuaternion operator-(const FQuaternion& otherQuat) const;
        FQuaternion& operator-=(const FQuaternion& otherQuat);
        float operator|(const FQuaternion& otherQuat) const;
        FQuaternion& operator*=(const float scale);
        FQuaternion operator*(const float scale) const;
        FQuaternion operator*(const FQuaternion& otherQuat) const;
        FQuaternion& operator*=(const FQuaternion& otherQuat);
        FVector3 operator*(const FVector3& toMultiply) const;
        FMatrix3 operator*(const FMatrix3& multiply) const;
        FQuaternion& operator/=(const float scale);
        FQuaternion operator/(const float scale) const;

        static FQuaternion LookAt(const FVector3& forward, const FVector3& up);
        static bool IsIdentity(const FQuaternion& target);
        static bool IsPure(const FQuaternion& target);
        static bool IsNormalized(const FQuaternion& target);
        static float DotProduct(const FQuaternion& left, const FQuaternion& right);
        static FQuaternion Normalize(const FQuaternion& target);
        static float Length(const FQuaternion& target);
        static float LengthSquare(const FQuaternion& target);
        static float GetAngle(const FQuaternion& target);
        static FVector3 GetRotationAxis(const FQuaternion& target);
        static FQuaternion Inverse(const FQuaternion& target);
        static FQuaternion Conjugate(const FQuaternion& target);
        static FQuaternion Square(const FQuaternion& target);
        static std::pair<FVector3, float> GetAxisAndAngle(const FQuaternion& target);
        static float AngularDistance(const FQuaternion& left, const FQuaternion& right);
        static FQuaternion Lerp(const FQuaternion& start, const FQuaternion& end, float alpha);
        static FQuaternion Slerp(const FQuaternion& start, const FQuaternion& end, float alpha);
        static FQuaternion Nlerp(const FQuaternion& start, const FQuaternion& end, float alpha);
        static FVector3 RotatePoint(const FVector3& point, const FQuaternion& quaternion);
        static FVector3 RotatePoint(const FVector3& point, const FQuaternion& quaternion, const FVector3& pivot);
        static FVector3 EulerAngles(const FQuaternion& target);
        static FMatrix3 ToMatrix3(const FQuaternion& target);
        static FMatrix4 ToMatrix4(const FQuaternion& target);
    };
}
}

#endif // ENGINE_MATH_QUATERNION_HPP_
