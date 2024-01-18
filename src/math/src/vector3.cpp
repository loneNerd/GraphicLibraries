#include "math/vector3.hpp"

#include <cmath>
#include <stdexcept>
#include <utility>

namespace Math = Engine::Math;

//const Math::FVector3 Math::FVector3::One(1.0f, 1.0f, 1.0f);
//const Math::FVector3 Math::FVector3::Zero(0.0f, 0.0f, 0.0f);
//const Math::FVector3 Math::FVector3::Forward(0.0f, 0.0f, 1.0f);
//const Math::FVector3 Math::FVector3::Right(1.0f, 0.0f, 0.0f);
//const Math::FVector3 Math::FVector3::Up(0.0f, 1.0f, 0.0f);

Math::FVector3 Math::FVector3::operator-() const
{
    return operator*(-1);
}

Math::FVector3 Math::FVector3::operator=(const Math::FVector3& other)
{
    this->X = other.X;
    this->Y = other.Y;
    this->Z = other.Z;

    return *this;
}

Math::FVector3 Math::FVector3::operator+(const Math::FVector3& other) const
{
    return Add(*this, other);
}

Math::FVector3& Math::FVector3::operator+=(const Math::FVector3& other)
{
    *this = Add(*this, other);
    return *this;
}

Math::FVector3 Math::FVector3::operator-(const Math::FVector3& other) const
{
    return Substract(*this, other);
}

Math::FVector3& Math::FVector3::operator-=(const Math::FVector3& other)
{
    *this = Substract(*this, other);
    return *this;
}

Math::FVector3 Math::FVector3::operator*(float scalar) const
{
    return Multiply(*this, scalar);
}

Math::FVector3& Math::FVector3::operator*=(float scalar)
{
    *this = Multiply(*this, scalar);
    return *this;
}

Math::FVector3 Math::FVector3::operator/(float scalar) const
{
    return Divide(*this, scalar);
}

Math::FVector3& Math::FVector3::operator/=(float scalar)
{
    *this = Divide(*this, scalar);
    return *this;
}

bool Math::FVector3::operator==(const Math::FVector3& other)
{
    return
        this->X == other.X &&
        this->Y == other.Y &&
        this->Z == other.Z;
}

bool Math::FVector3::operator!=(const Math::FVector3& other)
{
    return !operator==(other);
}

Math::FVector3 Math::FVector3::Add(const Math::FVector3& left, const Math::FVector3& right)
{
    return FVector3
    (
        left.X + right.X,
        left.Y + right.Y,
        left.Z + right.Z
    );
}

Math::FVector3 Math::FVector3::Substract(const Math::FVector3& left, const Math::FVector3& right)
{
    return FVector3
    (
        left.X - right.X,
        left.Y - right.Y,
        left.Z - right.Z
    );
}

Math::FVector3 Math::FVector3::Multiply(const Math::FVector3& target, float scalar)
{
    return FVector3
    (
        target.X * scalar,
        target.Y * scalar,
        target.Z * scalar
    );
}

Math::FVector3 Math::FVector3::Divide(const Math::FVector3& left, float scalar)
{
    Math::FVector3 result(left);

    if (scalar == 0)
        throw std::logic_error("Division by 0");

    result.X /= scalar;
    result.Y /= scalar;
    result.Z /= scalar;

    return result;
}

float Math::FVector3::Length(const Math::FVector3& target)
{
    return std::sqrt(target.X * target.X + target.Y * target.Y + target.Z * target.Z);
}

float Math::FVector3::Dot(const Math::FVector3& left, const Math::FVector3& right)
{
    return left.X * right.X + left.Y * right.Y + left.Z * right.Z;
}

float Math::FVector3::Distance(const Math::FVector3& left, const Math::FVector3& right)
{
    return std::sqrt
    (
        (left.X - right.X) * (left.X - right.X) +
        (left.Y - right.Y) * (left.Y - right.Y) +
        (left.Z - right.Z) * (left.Z - right.Z)
    );
}

Math::FVector3 Math::FVector3::Cross(const Math::FVector3& left, const Math::FVector3& right)
{
    return FVector3
    (
        left.Y * right.Z - left.Z * right.Y,
        left.Z * right.X - left.X * right.Z,
        left.X * right.Y - left.Y * right.X
    );
}

Math::FVector3 Math::FVector3::Normalize(const Math::FVector3& target)
{
    float length = Length(target);

    if (length > 0.0f)
    {
        float targetLength = 1.0f / length;

        return FVector3
        (
            target.X * targetLength,
            target.Y * targetLength,
            target.Z * targetLength
        );
    }
    else
    {
        return Math::FVector3(0.0f, 0.0f, 0.0f);
    }
}

Math::FVector3 Math::FVector3::Lerp(const Math::FVector3& start, const Math::FVector3& end, float alpha)
{
    return (start + (end - start) * alpha);
}

float Math::FVector3::AngleBetween(const Math::FVector3& from, const Math::FVector3& to)
{
    float lengthProduct = Length(from) * Length(to);

    if (lengthProduct > 0.0f)
    {
        float fractionResult = Dot(from, to) / lengthProduct;

        if (fractionResult >= -1.0f && fractionResult <= 1.0f)
            return acosf(fractionResult);
    }

    return 0.0f;
}
