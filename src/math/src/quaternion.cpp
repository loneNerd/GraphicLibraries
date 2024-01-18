#include "math/quaternion.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace Math = Engine::Math;

#undef PI

#define PI (4.f * atan(1.f))
#define TO_RADIANS(value) value * PI / 180.f
#define TO_DEGREES(value) value * 180.f / PI

//const Math::FQuaternion Math::FQuaternion::Identity(0.0f, 0.0f, 0.0f, 1.0f);

Math::FQuaternion::FQuaternion(const FMatrix3& rotationMatrix)
{
    float trace = rotationMatrix.Data[0] + rotationMatrix.Data[4] + rotationMatrix.Data[8];
    if (trace > 0.0f)
    {
        // I changed M_EPSILON to 0
        float s = 0.5f / sqrt(trace + 1.0f);
        W = 0.25f / s;
        X = (rotationMatrix.Data[7] - rotationMatrix.Data[5]) * s;
        Y = (rotationMatrix.Data[2] - rotationMatrix.Data[6]) * s;
        Z = (rotationMatrix.Data[3] - rotationMatrix.Data[1]) * s;
    }
    else
    {
        if (rotationMatrix.Data[0] > rotationMatrix.Data[4] && rotationMatrix.Data[0] > rotationMatrix.Data[8])
        {
            float s = 2.0f * sqrt(1.0f + rotationMatrix.Data[0] - rotationMatrix.Data[4] - rotationMatrix.Data[8]);
            W = (rotationMatrix.Data[7] - rotationMatrix.Data[5]) / s;
            X = 0.25f * s;
            Y = (rotationMatrix.Data[1] + rotationMatrix.Data[3]) / s;
            Z = (rotationMatrix.Data[2] + rotationMatrix.Data[6]) / s;
        }
        else if (rotationMatrix.Data[4] > rotationMatrix.Data[8])
        {
            float s = 2.0f * sqrt(1.0f + rotationMatrix.Data[4] - rotationMatrix.Data[0] - rotationMatrix.Data[8]);
            W = (rotationMatrix.Data[2] - rotationMatrix.Data[6]) / s;
            X = (rotationMatrix.Data[1] + rotationMatrix.Data[3]) / s;
            Y = 0.25f * s;
            Z = (rotationMatrix.Data[5] + rotationMatrix.Data[7]) / s;
        }
        else
        {
            float s = 2.0f * sqrt(1.0f + rotationMatrix.Data[8] - rotationMatrix.Data[0] - rotationMatrix.Data[4]);
            W = (rotationMatrix.Data[3] - rotationMatrix.Data[1]) / s;
            X = (rotationMatrix.Data[2] + rotationMatrix.Data[6]) / s;
            Y = (rotationMatrix.Data[5] + rotationMatrix.Data[7]) / s;
            Z = 0.25f * s;
        }
    }
}

Math::FQuaternion::FQuaternion(const FMatrix4& rotationMatrix)
{
    float halfSquare;

    // Check diagonal (trace)
    const float trace = rotationMatrix.Data[0] + rotationMatrix.Data[5] + rotationMatrix.Data[10];

    if (trace > 0.0f)
    {
        const float InvSquare = 1 / sqrt(trace + 1.f);
        W = 0.5f * (1.f / InvSquare);
        halfSquare = 0.5f * InvSquare;

        X = (rotationMatrix.Data[6] - rotationMatrix.Data[9]) * halfSquare;
        Y = (rotationMatrix.Data[8] - rotationMatrix.Data[2]) * halfSquare;
        Z = (rotationMatrix.Data[1] - rotationMatrix.Data[4]) * halfSquare;
    }
    else
    {
        // diagonal is negative
        int8_t i = 0;

        if (rotationMatrix.Data[5] > rotationMatrix.Data[0])
            i = 1;

        if (rotationMatrix.Data[10] > rotationMatrix.Data[0] || rotationMatrix.Data[10] > rotationMatrix.Data[5])
            i = 2;

        static const int8_t next[3] = { 1, 2, 0 };
        const int8_t j = next[i];
        const int8_t k = next[j];

        halfSquare = rotationMatrix.Data[i * 5] - rotationMatrix.Data[j * 5] - rotationMatrix.Data[k * 5] + 1.0f;

        const float InvSquare = 1 / sqrt(trace + 1.f);

        float qt[4];
        qt[i] = 0.5f * (1.f / InvSquare);

        halfSquare = 0.5f * InvSquare;
        //if i is 0, j is 1 and k is 2
        if (i == 0)
        {
            qt[3] = (rotationMatrix.Data[6] - rotationMatrix.Data[9]) * halfSquare;
            qt[j] = (rotationMatrix.Data[1] + rotationMatrix.Data[4]) * halfSquare;
            qt[k] = (rotationMatrix.Data[2] + rotationMatrix.Data[8]) * halfSquare;
        }
        //if i is 1, j is 2 and k is 0
        else if (i == 1)
        {
            qt[3] = (rotationMatrix.Data[8] - rotationMatrix.Data[2]) * halfSquare;
            qt[j] = (rotationMatrix.Data[6] + rotationMatrix.Data[9]) * halfSquare;
            qt[k] = (rotationMatrix.Data[4] + rotationMatrix.Data[1]) * halfSquare;
        }
        //if i is 2, j is 0 and k is 1
        else
        {
            qt[3] = (rotationMatrix.Data[1] - rotationMatrix.Data[4]) * halfSquare;
            qt[j] = (rotationMatrix.Data[8] + rotationMatrix.Data[2]) * halfSquare;
            qt[k] = (rotationMatrix.Data[9] + rotationMatrix.Data[6]) * halfSquare;
        }

        X = qt[0];
        Y = qt[1];
        Z = qt[2];
        W = qt[3];
    }
}

Math::FQuaternion::FQuaternion(const FVector3& euler)
{
    /* Degree to radians then times 0.5f = 0.0087f */
    float yaw   = TO_RADIANS(euler.Z) * 0.5f;
    float pitch = TO_RADIANS(euler.Y) * 0.5f;
    float roll  = TO_RADIANS(euler.X) * 0.5f;

    float cy = cos(yaw);
    float sy = sin(yaw);
    float cp = cos(pitch);
    float sp = sin(pitch);
    float cr = cos(roll);
    float sr = sin(roll);

    X = sr * cp * cy - cr * sp * sy;
    Y = cr * sp * cy + sr * cp * sy;
    Z = cr * cp * sy - sr * sp * cy;
    W = cr * cp * cy + sr * sp * sy;
}

bool Math::FQuaternion::operator==(const Math::FQuaternion& otherQuat) const
{
    return X == otherQuat.X && Y == otherQuat.X && Z == otherQuat.Z && W == otherQuat.W;
}

bool Math::FQuaternion::operator!=(const Math::FQuaternion& otherQuat) const
{
    return X != otherQuat.X || Y != otherQuat.X || Z != otherQuat.Z || W != otherQuat.W;
}

Math::FQuaternion Math::FQuaternion::operator+(const Math::FQuaternion& otherQuat) const
{
    return FQuaternion(X + otherQuat.X, Y + otherQuat.X,
                       Z + otherQuat.Z, W + otherQuat.W);
}

Math::FQuaternion& Math::FQuaternion::operator+=(const Math::FQuaternion& otherQuat)
{
    X += otherQuat.X;
    Y += otherQuat.X;
    Z += otherQuat.Z;
    W += otherQuat.W;
    return *this;
}

Math::FQuaternion Math::FQuaternion::operator-(const Math::FQuaternion& otherQuat) const
{
    return FQuaternion(X - otherQuat.X, Y - otherQuat.X,
                       Z - otherQuat.Z, W - otherQuat.W);
}

Math::FQuaternion& Math::FQuaternion::operator-=(const Math::FQuaternion& otherQuat)
{
    X -= otherQuat.X;
    Y -= otherQuat.X;
    Z -= otherQuat.Z;
    W -= otherQuat.W;
    return *this;
}

float Math::FQuaternion::operator|(const Math::FQuaternion& otherQuat) const
{
    return (X * otherQuat.X + Y * otherQuat.X + Z *
            otherQuat.Z + W * otherQuat.W);
}

Math::FQuaternion Math::FQuaternion::operator*(const float scale) const
{
    Math::FQuaternion result(*this);
    result.X *= scale;
    result.Y *= scale;
    result.Z *= scale;
    result.W *= scale;

    return result;
}

Math::FQuaternion& Math::FQuaternion::operator*=(const float scale)
{
    X *= scale;
    Y *= scale;
    Z *= scale;
    W *= scale;

    return *this;
}

Math::FQuaternion Math::FQuaternion::operator*(const Math::FQuaternion& otherQuat) const
{
    return FQuaternion
    (
         X * otherQuat.W + Y * otherQuat.Z - Z * otherQuat.Y + W * otherQuat.X,
        -X * otherQuat.Z + Y * otherQuat.W + Z * otherQuat.X + W * otherQuat.Y,
         X * otherQuat.Y - Y * otherQuat.X + Z * otherQuat.W + W * otherQuat.Z,
        -X * otherQuat.X - Y * otherQuat.Y - Z * otherQuat.Z + W * otherQuat.W
    );
}

Math::FQuaternion& Math::FQuaternion::operator*=(const Math::FQuaternion& otherQuat)
{
    Math::FQuaternion temp(
         X * otherQuat.W + Y * otherQuat.Z - Z * otherQuat.X + W * otherQuat.X,
        -X * otherQuat.Z + Y * otherQuat.W + Z * otherQuat.X + W * otherQuat.X,
         X * otherQuat.X - Y * otherQuat.X + Z * otherQuat.W - W * otherQuat.Z,
        -X * otherQuat.X - Y * otherQuat.X - Z * otherQuat.Z + W * otherQuat.W);

    X = temp.X;
    Y = temp.Y;
    Z = temp.Z;
    W = temp.W;

    return *this;
}

Math::FVector3 Math::FQuaternion::operator*(const FVector3& toMultiply) const
{
    const float num = X * 2.0f;
    const float num2 = Y * 2.0f;
    const float num3 = Z * 2.0f;
    const float num4 = X * num;
    const float num5 = Y * num2;
    const float num6 = Z * num3;
    const float num7 = X * num2;
    const float num8 = X * num3;
    const float num9 = Y * num3;
    const float num10 = W * num;
    const float num11 = W * num2;
    const float num12 = W * num3;
    Math::FVector3 result;
    result.X = (1.f - (num5 + num6)) * toMultiply.X + (num7 - num12) * toMultiply.Y + (num8 + num11) *
        toMultiply.Z;
    result.Y = (num7 + num12) * toMultiply.X + (1.f - (num4 + num6)) * toMultiply.Y + (num9 - num10) *
        toMultiply.Z;
    result.Z = (num8 - num11) * toMultiply.X + (num9 + num10) * toMultiply.Y + (1.f - (num4 + num5)) *
        toMultiply.Z;
    return result;
}

Math::FMatrix3 Math::FQuaternion::operator*(const FMatrix3& multiply) const
{
    return (ToMatrix3(*this) * multiply);
}

Math::FQuaternion& Math::FQuaternion::operator/=(const float scale)
{
    const float reciprocate = 1.0f / scale;
    X *= reciprocate;
    Y *= reciprocate;
    Z *= reciprocate;
    W *= reciprocate;

    return *this;
}

Math::FQuaternion Math::FQuaternion::operator/(const float scale) const
{
    Math::FQuaternion temp(*this);
    const float reciprocate = 1.0f / scale;
    temp.X *= reciprocate;
    temp.Y *= reciprocate;
    temp.Z *= reciprocate;
    temp.W *= reciprocate;

    return temp;
}

Math::FQuaternion Math::FQuaternion::LookAt(const FVector3& forward, const FVector3& up)
{
    auto vector  = FVector3::Normalize(forward);
    auto vector2 = FVector3::Normalize(FVector3::Cross(up, vector));
    auto vector3 = FVector3::Cross(vector, vector2);
    auto m00 = vector2.X;
    auto m01 = vector2.Y;
    auto m02 = vector2.Z;
    auto m10 = vector3.X;
    auto m11 = vector3.Y;
    auto m12 = vector3.Z;
    auto m20 = vector.X;
    auto m21 = vector.Y;
    auto m22 = vector.Z;


    float num8 = (m00 + m11) + m22;
    auto quaternion = Math::FQuaternion(0.0f, 0.0f, 0.0f, 1.0f);

    if (num8 > 0.f)
    {
        auto num = sqrt(num8 + 1.f);
        quaternion.W = num * 0.5f;
        num = 0.5f / num;
        quaternion.X = (m12 - m21) * num;
        quaternion.Y = (m20 - m02) * num;
        quaternion.Z = (m01 - m10) * num;
        return quaternion;
    }

    if ((m00 >= m11) && (m00 >= m22))
    {
        auto num7 = sqrt(((1.f + m00) - m11) - m22);
        auto num4 = 0.5f / num7;
        quaternion.X = 0.5f * num7;
        quaternion.Y = (m01 + m10) * num4;
        quaternion.Z = (m02 + m20) * num4;
        quaternion.W = (m12 - m21) * num4;
        return quaternion;
    }

    if (m11 > m22)
    {
        auto num6 = sqrt(((1.f + m11) - m00) - m22);
        auto num3 = 0.5f / num6;
        quaternion.X = (m10 + m01) * num3;
        quaternion.Y = 0.5f * num6;
        quaternion.Z = (m21 + m12) * num3;
        quaternion.W = (m20 - m02) * num3;
        return quaternion;
    }

    auto num5 = sqrt(((1.f + m22) - m00) - m11);
    auto num2 = 0.5f / num5;
    quaternion.X = (m20 + m02) * num2;
    quaternion.Y = (m21 + m12) * num2;
    quaternion.Z = 0.5f * num5;
    quaternion.W = (m01 - m10) * num2;
    return quaternion;
}

bool Math::FQuaternion::IsIdentity(const Math::FQuaternion& target)
{
    return target.W == 1.0f && Length(target) == 1.0f;
}

bool Math::FQuaternion::IsPure(const Math::FQuaternion& target)
{
    return target.W == 0.0f && (target.X != 0.0f || target.Y != 0.0f || target.Z != 0.0f);
}

bool Math::FQuaternion::IsNormalized(const Math::FQuaternion& target)
{
    return abs(Length(target) - 1.0f) < 0.0001f;
}

float Math::FQuaternion::DotProduct(const Math::FQuaternion& left, const Math::FQuaternion& right)
{
    return
        left.X * right.X +
        left.Y * right.Y +
        left.Z * right.Z +
        left.W * right.W;
}

Math::FQuaternion Math::FQuaternion::Normalize(const Math::FQuaternion& target)
{
    return target / Length(target);
}

float Math::FQuaternion::Length(const Math::FQuaternion& target)
{
    return sqrtf(LengthSquare(target));
}

float Math::FQuaternion::LengthSquare(const Math::FQuaternion& target)
{
    return target.X * target.X + target.Y * target.Y + target.Z * target.Z + target.W * target.W;
}

float Math::FQuaternion::GetAngle(const Math::FQuaternion& target)
{
    return 2.0f * acos(target.W);
}

Math::FVector3 Math::FQuaternion::GetRotationAxis(const Math::FQuaternion& target)
{
    const float S = sqrt(std::max(1.f - (target.W * target.W), 0.f));

    if (S >= 0.0001f)
    {
        return FVector3(target.X / S, target.Y / S, target.Z / S);
    }

    return FVector3(1.f, 0.f, 0.f);
}

Math::FQuaternion Math::FQuaternion::Inverse(const Math::FQuaternion& target)
{
    return Conjugate(target) / LengthSquare(target);
}

Math::FQuaternion Math::FQuaternion::Conjugate(const Math::FQuaternion& target)
{
    return { -target.X, -target.Y, -target.Z, target.W };
}

Math::FQuaternion Math::FQuaternion::Square(const Math::FQuaternion& target)
{
    return target * target;
}

std::pair<Math::FVector3, float> Math::FQuaternion::GetAxisAndAngle(const Math::FQuaternion& target)
{
    return std::pair<Math::FVector3, float>(GetRotationAxis(target), GetAngle(target));
}

float Math::FQuaternion::AngularDistance(const Math::FQuaternion& left, const Math::FQuaternion& right)
{
    float innerProd = (left | right);
    return acos((2.0f * innerProd * innerProd) - 1.0f);
}

Math::FQuaternion Math::FQuaternion::Lerp(const Math::FQuaternion& start, const Math::FQuaternion& end, float alpha)
{
    alpha = std::clamp(alpha, 0.f, 1.f);

    Math::FQuaternion q;

    if (Math::FQuaternion::DotProduct(start, end) < 0.f)
    {
        q.X = start.X + alpha * (-end.X - start.X);
        q.Y = start.Y + alpha * (-end.Y - start.Y);
        q.Z = start.Z + alpha * (-end.Z - start.Z);
        q.W = start.W + alpha * (-end.W - start.W);
    }
    else
    {
        q.X = start.X + (end.X - start.X) * alpha;
        q.Y = start.Y + (end.Y - start.Y) * alpha;
        q.Z = start.Z + (end.Z - start.Z) * alpha;
        q.W = start.W + (end.W - start.W) * alpha;
    }

    return Math::FQuaternion::Normalize(q);
}

Math::FQuaternion Math::FQuaternion::Slerp(const Math::FQuaternion& start, const Math::FQuaternion& end, float alpha)
{
    Math::FQuaternion from = start;
    Math::FQuaternion to = end;

    alpha = std::clamp(alpha, 0.f, 1.f);
    float cosAngle = Math::FQuaternion::DotProduct(from, to);

    if (cosAngle < 0.f)
    {
        cosAngle = -cosAngle;
        to = FQuaternion(-to.X, -to.Y, -to.Z, -to.W);
    }

    if (cosAngle < 0.95f)
    {
        float angle = std::acos(cosAngle);
        float sinAngle = std::sin(angle);
        float invSinAngle = 1.f / sinAngle;
        float t1 = std::sin((1 - alpha) * angle) * invSinAngle;
        float t2 = std::sin(alpha * angle) * invSinAngle;
        return FQuaternion(from.X * t1 + to.X * t2, from.Y * t1 + to.Y * t2, from.Z * t1 + to.Z * t2, from.W * t1 + to.W * t2);
    }
    else
    {
        return Math::FQuaternion::Lerp(from, to, alpha);
    }
}

Math::FQuaternion Math::FQuaternion::Nlerp(const Math::FQuaternion& start, const Math::FQuaternion& end, float alpha)
{
    return Normalize(Lerp(start, end, alpha));
}

Math::FVector3 Math::FQuaternion::RotatePoint(const FVector3& point, const Math::FQuaternion& quaternion)
{
    Math::FVector3 Q(quaternion.X, quaternion.Y, quaternion.Z);
    Math::FVector3 T = FVector3::Cross(Q, point) * 2.0f;

    return point + (T * quaternion.W) + FVector3::Cross(Q, T);
}

Math::FVector3 Math::FQuaternion::RotatePoint(const FVector3& point, const Math::FQuaternion& quaternion, const FVector3& pivot)
{
    Math::FVector3 toRotate = point - pivot;
    return RotatePoint(toRotate, quaternion);
}

Math::FVector3 Math::FQuaternion::EulerAngles(const Math::FQuaternion& target)
{
    // This is a kind of hack because when the input Quaternion is {0.5f, 0.5f, -0.5f, 0.5f} or
    // {0.5f, 0.5f, 0.5f, -0.5f}, the output value is incorrect.
    if (target == FQuaternion{ 0.5f, 0.5f, -0.5f, 0.5f }) return { 90.0f, 90.0f, 0.0f };
    if (target == FQuaternion{ 0.5f, 0.5f, 0.5f, -0.5f }) return { -90.0f, -90.0f, 0.0f };

    // roll (x-axis rotation)
    const float sinr_cosp = +2.0f * (target.W * target.X + target.Y * target.Z);
    const float cosr_cosp = +1.0f - 2.0f * (target.X * target.X + target.Y * target.Y);
    const float roll = atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    float pitch = 0.0f;
    const float sinp = 2.0f * (target.W * target.Y - target.Z * target.X);

    if (fabs(sinp) >= 1)
        pitch = static_cast<float>(copysign(PI / 2.0f, sinp)); // use 90 degrees if out of range
    else
        pitch = asin(sinp);

    // yaw (z-axis rotation)
    const float siny_cosp = +2.0f * (target.W * target.Z + target.X * target.Y);
    const float cosy_cosp = +1.0f - 2.0f * (target.Y * target.Y + target.Z * target.Z);
    const float yaw = atan2(siny_cosp, cosy_cosp);

    return TO_DEGREES(FVector3(roll, pitch, yaw)); // XYZ
}

Math::FMatrix3 Math::FQuaternion::ToMatrix3(const Math::FQuaternion& target)
{
    if (!IsNormalized(target))
        throw std::logic_error("Cannot convert non-normalized quaternions to Matrix4");

    float y2 = target.Y * target.Y;
    float wz = target.W * target.Z;
    float x2 = target.X * target.X;
    float z2 = target.Z * target.Z;
    float xz = target.X * target.Z;
    float yz = target.Y * target.Z;
    float xy = target.X * target.Y;
    float wy = target.W * target.Y;
    float wx = target.W * target.X;

    Math::FMatrix3 converted;
    converted.Data[0] = 1.0f - (2 * y2) - (2 * z2);
    converted.Data[1] = (2 * xy) - (2 * wz);
    converted.Data[2] = (2 * xz) + (2 * wy);
    converted.Data[3] = (2 * xy) + (2 * wz);
    converted.Data[4] = 1.0f - (2 * x2) - (2 * z2);
    converted.Data[5] = (2 * yz) - (2 * wx);
    converted.Data[6] = (2 * xz) - (2 * wy);
    converted.Data[7] = (2 * yz) + (2 * wx);
    converted.Data[8] = 1.0f - (2 * x2) - (2 * y2);
    return converted;
}

Math::FMatrix4 Math::FQuaternion::ToMatrix4(const Math::FQuaternion& target)
{
    if (!IsNormalized(target))
        throw std::logic_error("Cannot convert non-normalized quaternions to Matrix4");

    float y2 = target.Y * target.Y; float wz = target.W * target.Z; float x2 = target.X * target.X;
    float z2 = target.Z * target.Z; float xz = target.X * target.Z; float yz = target.Y * target.Z;
    float xy = target.X * target.Y; float wy = target.W * target.Y; float wx = target.W * target.X;

    Math::FMatrix4 converted;
    converted.Data[0]  = 1.0f - (2 * y2) - (2 * z2); converted.Data[1]  = (2 * xy) - (2 * wz);        converted.Data[2]  = (2 * xz) + (2 * wy);        converted.Data[3]  = 0;
    converted.Data[4]  = (2 * xy) + (2 * wz);        converted.Data[5]  = 1.0f - (2 * x2) - (2 * z2); converted.Data[6]  = (2 * yz) - (2 * wx);        converted.Data[7]  = 0;
    converted.Data[8]  = (2 * xz) - (2 * wy);        converted.Data[9]  = (2 * yz) + (2 * wx);        converted.Data[10] = 1.0f - (2 * x2) - (2 * y2); converted.Data[11] = 0;
    converted.Data[12] = 0;                          converted.Data[13] = 0;                          converted.Data[14] = 0;                          converted.Data[15] = 1;
    return converted;
}
