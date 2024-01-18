#include "math/vector4.hpp"

#include <stdexcept>
#include <utility>

namespace Math = Engine::Math;

const Math::FVector4 Math::FVector4::One(1.0f, 1.0f, 1.0f, 1.0f);
const Math::FVector4 Math::FVector4::Zero(0.0f, 0.0f, 0.0f, 0.0f);

Math::FVector4 Math::FVector4::operator-() const
{
    return operator*(-1);
}

Math::FVector4 Math::FVector4::operator=(const Math::FVector4& other)
{
    this->X = other.X;
    this->Y = other.Y;
    this->Z = other.Z;
    this->W = other.W;

    return *this;
}

Math::FVector4 Math::FVector4::operator+(const Math::FVector4& other) const
{
    return Add(*this, other);
}

Math::FVector4& Math::FVector4::operator+=(const Math::FVector4& other)
{
    *this = Add(*this, other);
    return *this;
}

Math::FVector4 Math::FVector4::operator-(const Math::FVector4& other) const
{
    return Substract(*this, other);
}

Math::FVector4& Math::FVector4::operator-=(const Math::FVector4& other)
{
    *this = Substract(*this, other);
    return *this;
}

Math::FVector4 Math::FVector4::operator*(float scalar) const
{
    return Multiply(*this, scalar);
}

Math::FVector4& Math::FVector4::operator*=(float scalar)
{
    *this = Multiply(*this, scalar);
    return *this;
}

Math::FVector4 Math::FVector4::operator/(float scalar) const
{
    return Divide(*this, scalar);
}

Math::FVector4& Math::FVector4::operator/=(float scalar)
{
    *this = Divide(*this, scalar);
    return *this;
}

bool Math::FVector4::operator==(const Math::FVector4& other)
{
    return
        this->X == other.X &&
        this->Y == other.Y &&
        this->Z == other.Z &&
        this->W == other.W;
}

bool Math::FVector4::operator!=(const Math::FVector4& other)
{
    return !operator==(other);
}

Math::FVector4 Math::FVector4::Add(const Math::FVector4& left, const Math::FVector4& right)
{
    return FVector4
    (
        left.X + right.X,
        left.Y + right.Y,
        left.Z + right.Z,
        left.W + right.W
    );
}

Math::FVector4 Math::FVector4::Substract(const Math::FVector4& left, const Math::FVector4& right)
{
    return FVector4
    (
        left.X - right.X,
        left.Y - right.Y,
        left.Z - right.Z,
        left.W - right.W
    );
}

Math::FVector4 Math::FVector4::Multiply(const Math::FVector4& target, float scalar)
{
    return FVector4
    (
        target.X * scalar,
        target.Y * scalar,
        target.Z * scalar,
        target.W * scalar
    );
}

Math::FVector4 Math::FVector4::Divide(const Math::FVector4& left, float scalar)
{
    Math::FVector4 result(left);

    if (scalar == 0)
        throw std::logic_error("Division by 0");

    result.X /= scalar;
    result.Y /= scalar;
    result.Z /= scalar;
    result.W /= scalar;

    return result;
}

float Math::FVector4::Length(const Math::FVector4& target)
{
    return sqrtf(target.X * target.X + target.Y * target.Y + target.Z * target.Z + target.W * target.W);
}

float Math::FVector4::Dot(const Math::FVector4& left, const Math::FVector4& right)
{
    return left.X * right.X + left.Y * right.Y + left.Z * right.Z + left.W * right.W;
}

Math::FVector4 Math::FVector4::Normalize(const Math::FVector4& target)
{
    float length = Length(target);

    if (length > 0.0f)
    {
        float targetLength = 1.0f / length;

        return FVector4
        (
            target.X * targetLength,
            target.Y * targetLength,
            target.Z * targetLength,
            target.W * targetLength
        );
    }
    else
    {
        return Math::FVector4::Zero;
    }
}

Math::FVector4 Math::FVector4::Lerp(const Math::FVector4& start, const Math::FVector4& end, float alpha)
{
    return (start + (end - start) * alpha);
}
