#include "vector2.hpp"

#include <stdexcept>
#include <utility>

namespace Math = Engine::Math;

const Math::FVector2 Math::FVector2::One(1.0f, 1.0f);
const Math::FVector2 Math::FVector2::Zero(0.0f, 0.0f);

Math::FVector2 Math::FVector2::operator-() const
{
    return operator*(-1);
}

Math::FVector2 Math::FVector2::operator=(const Math::FVector2& other)
{
    this->X = other.X;
    this->Y = other.Y;

    return *this;
}

Math::FVector2 Math::FVector2::operator+(const Math::FVector2& other) const
{
    return Add(*this, other);
}

Math::FVector2& Math::FVector2::operator+=(const Math::FVector2& other)
{
    *this = Add(*this, other);
    return *this;
}

Math::FVector2 Math::FVector2::operator-(const Math::FVector2& other) const
{
    return Substract(*this, other);
}

Math::FVector2& Math::FVector2::operator-=(const Math::FVector2& other)
{
    *this = Substract(*this, other);
    return *this;
}

Math::FVector2 Math::FVector2::operator*(float scalar) const
{
    return Multiply(*this, scalar);
}

Math::FVector2& Math::FVector2::operator*=(float scalar)
{
    *this = Multiply(*this, scalar);
    return *this;
}

Math::FVector2 Math::FVector2::operator/(float scalar) const
{
    return Divide(*this, scalar);
}

Math::FVector2& Math::FVector2::operator/=(float scalar)
{
    *this = Divide(*this, scalar);
    return *this;
}

bool Math::FVector2::operator==(const Math::FVector2& other)
{
    return
        this->X == other.X &&
        this->Y == other.Y;
}

bool Math::FVector2::operator!=(const Math::FVector2& other)
{
    return !operator==(other);
}

Math::FVector2 Math::FVector2::Add(const Math::FVector2& left, const Math::FVector2& right)
{
    return FVector2
    (
        left.X + right.X,
        left.Y + right.Y
    );
}

Math::FVector2 Math::FVector2::Substract(const Math::FVector2& left, const Math::FVector2& right)
{
    return FVector2
    (
        left.X - right.X,
        left.Y - right.Y
    );
}

Math::FVector2 Math::FVector2::Multiply(const Math::FVector2& target, float scalar)
{
    return FVector2
    (
        target.X * scalar,
        target.Y * scalar
    );
}

Math::FVector2 Math::FVector2::Divide(const Math::FVector2& left, float scalar)
{
    Math::FVector2 result(left);

    if (scalar == 0)
        throw std::logic_error("Division by 0");

    result.X /= scalar;
    result.Y /= scalar;

    return result;
}

float Math::FVector2::Length(const Math::FVector2& target)
{
    return sqrtf(target.X * target.X + target.Y * target.Y);
}

float Math::FVector2::Dot(const Math::FVector2& left, const Math::FVector2& right)
{
    return left.X * right.X + left.Y * right.Y;
}

Math::FVector2 Math::FVector2::Normalize(const Math::FVector2& target)
{
    float length = Length(target);

    if (length > 0.0f)
    {
        float targetLength = 1.0f / length;

        return FVector2
        (
            target.X * targetLength,
            target.Y * targetLength
        );
    }
    else
        return Math::FVector2::Zero;
}

Math::FVector2 Math::FVector2::Lerp(const Math::FVector2& start, const Math::FVector2& end, float alpha)
{
    return (start + (end - start) * alpha);
}

float Math::FVector2::AngleBetween(const Math::FVector2& from, const Math::FVector2& to)
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
