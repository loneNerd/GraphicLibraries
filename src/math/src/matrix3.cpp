#include "math/matrix3.hpp"

#include <cmath>
#include <stdexcept>
#include <string>

namespace Math = Engine::Math;

const Math::FMatrix3 Math::FMatrix3::Identity = FMatrix3(1.0f, 0.0f, 0.0f,
                                                         0.0f, 1.0f, 0.0f,
                                                         0.0f, 0.0f, 1.0f);

Math::FMatrix3::FMatrix3()
{
    memcpy(Data.data(), Identity.Data.data(), Data.size() * sizeof(float));
}

Math::FMatrix3::FMatrix3(float value)
{
    for (float& element : Data)
        element = value;
}

Math::FMatrix3::FMatrix3(float element1, float element2, float element3, float element4, float element5, float element6, float element7, float element8, float element9)
{
    Data[0] = element1;
    Data[1] = element2;
    Data[2] = element3;
    Data[3] = element4;
    Data[4] = element5;
    Data[5] = element6;
    Data[6] = element7;
    Data[7] = element8;
    Data[8] = element9;
}

Math::FMatrix3::FMatrix3(const Math::FMatrix3& other)
{
    *this = other;
}

Math::FMatrix3& Math::FMatrix3::operator=(const Math::FMatrix3& other)
{
    memcpy(this->Data.data(), other.Data.data(), Data.size()  * sizeof(float));
    return *this;
}

bool Math::FMatrix3::operator==(const Math::FMatrix3& other)
{
    return AreEquals(*this, other);
}

Math::FMatrix3 Math::FMatrix3::operator+(const Math::FMatrix3& other) const
{
    return Add(*this, other);
}

Math::FMatrix3& Math::FMatrix3::operator+=(const Math::FMatrix3& other)
{
    *this = Add(*this, other);
    return *this;
}

Math::FMatrix3 Math::FMatrix3::operator-(const Math::FMatrix3& other) const
{
    return Subtract(*this, other);
}

Math::FMatrix3& Math::FMatrix3::operator-=(const Math::FMatrix3& other)
{
    *this = Subtract(*this, other);
    return *this;
}

Math::FMatrix3 Math::FMatrix3::operator*(float scalar) const
{
    return Multiply(*this, scalar);
}

Math::FMatrix3& Math::FMatrix3::operator*=(float scalar)
{
    *this = Multiply(*this, scalar);
    return *this;
}

Math::FVector3 Math::FMatrix3::operator*(const FVector3& vector) const
{
    return Multiply(*this, vector);
}

Math::FMatrix3 Math::FMatrix3::operator*(const Math::FMatrix3& other) const
{
    return Multiply(*this, other);
}

Math::FMatrix3& Math::FMatrix3::operator*=(const Math::FMatrix3& other)
{
    *this = Multiply(*this, other);
    return *this;
}

Math::FMatrix3 Math::FMatrix3::operator/(float scalar) const
{
    return Divide(*this, scalar);
}

Math::FMatrix3& Math::FMatrix3::operator/=(float scalar)
{
    *this = Divide(*this, scalar);
    return *this;
}

Math::FMatrix3 Math::FMatrix3::operator/(const Math::FMatrix3& other) const
{
    return Divide(*this, other);
}

Math::FMatrix3& Math::FMatrix3::operator/=(const Math::FMatrix3& other)
{
    *this = Divide(*this, other);
    return *this;
}

float& Math::FMatrix3::operator()(uint8_t row, uint8_t column)
{
    if (row >= 3 || column >= 3)
        throw std::out_of_range(
            "Invalid index : " + std::to_string(row) + "," + std::to_string(column) + " is out of range");
    return Data[3 * row + column];
}

bool Math::FMatrix3::AreEquals(const Math::FMatrix3& left, const Math::FMatrix3& right)
{
    return memcmp(&left, &right, 9 * sizeof(float)) == 0;
}

Math::FMatrix3 Math::FMatrix3::Add(const Math::FMatrix3& left, float scalar)
{
    Math::FMatrix3 result(left);
    for (uint8_t i = 0; i < 9; ++i)
        result.Data[i] += scalar;
    return result;
}

Math::FMatrix3 Math::FMatrix3::Add(const Math::FMatrix3& left, const Math::FMatrix3& right)
{
    Math::FMatrix3 result(left);
    for (uint8_t i = 0; i < 9; ++i)
        result.Data[i] += right.Data[i];
    return result;
}

Math::FMatrix3 Math::FMatrix3::Subtract(const Math::FMatrix3& left, float scalar)
{
    Math::FMatrix3 result(left);
    for (float& element : result.Data)
    {
        element -= scalar;
    }
    return result;
}

Math::FMatrix3 Math::FMatrix3::Subtract(const Math::FMatrix3& left, const Math::FMatrix3& right)
{
    Math::FMatrix3 result(left);
    for (uint8_t i = 0; i < 9; ++i)
        result.Data[i] -= right.Data[i];
    return result;
}

Math::FMatrix3 Math::FMatrix3::Multiply(const Math::FMatrix3& left, float scalar)
{
    Math::FMatrix3 result(left);
    for (float& element : result.Data)
    {
        element *= scalar;
    }
    return result;
}

Math::FVector3 Math::FMatrix3::Multiply(const Math::FMatrix3& matrix, const FVector3& vector)
{
    Math::FVector3 result;
    result.X = ((matrix.Data[0] * vector.X) + (matrix.Data[1] * vector.Y) + (matrix.Data[2] * vector.Z));
    result.Y = ((matrix.Data[3] * vector.X) + (matrix.Data[4] * vector.Y) + (matrix.Data[5] * vector.Z));
    result.Z = ((matrix.Data[6] * vector.X) + (matrix.Data[7] * vector.Y) + (matrix.Data[8] * vector.Z));

    return result;
}

Math::FMatrix3 Math::FMatrix3::Multiply(const Math::FMatrix3& left, const Math::FMatrix3& right)
{
    return FMatrix3(
        (left.Data[0] * right.Data[0]) + (left.Data[1] * right.Data[3]) + (left.Data[2] * right.Data[6]),
        (left.Data[0] * right.Data[1]) + (left.Data[1] * right.Data[4]) + (left.Data[2] * right.Data[7]),
        (left.Data[0] * right.Data[2]) + (left.Data[1] * right.Data[5]) + (left.Data[2] * right.Data[8]),

        (left.Data[3] * right.Data[0]) + (left.Data[4] * right.Data[3]) + (left.Data[5] * right.Data[6]),
        (left.Data[3] * right.Data[1]) + (left.Data[4] * right.Data[4]) + (left.Data[5] * right.Data[7]),
        (left.Data[3] * right.Data[2]) + (left.Data[4] * right.Data[5]) + (left.Data[5] * right.Data[8]),

        (left.Data[6] * right.Data[0]) + (left.Data[7] * right.Data[3]) + (left.Data[8] * right.Data[6]),
        (left.Data[6] * right.Data[1]) + (left.Data[7] * right.Data[4]) + (left.Data[8] * right.Data[7]),
        (left.Data[6] * right.Data[2]) + (left.Data[7] * right.Data[5]) + (left.Data[8] * right.Data[8]));
}

Math::FMatrix3 Math::FMatrix3::Divide(const Math::FMatrix3& left, float scalar)
{
    Math::FMatrix3 result(left);
    for (float& element : result.Data)
    {
        element /= scalar;
    }
    return result;
}

Math::FMatrix3 Math::FMatrix3::Divide(const Math::FMatrix3& left, const Math::FMatrix3& right)
{
    return left * Inverse(right);
}

bool Math::FMatrix3::IsIdentity(const Math::FMatrix3& matrix)
{
    return memcmp(Identity.Data.data(), matrix.Data.data(), matrix.Data.size() * sizeof(float)) == 0;
}

float Math::FMatrix3::Determinant(const Math::FMatrix3& matrix)
{
    return matrix.Data[0] * (matrix.Data[4] * matrix.Data[8] - matrix.Data[5] * matrix.Data[7])
         - matrix.Data[3] * (matrix.Data[1] * matrix.Data[8] - matrix.Data[2] * matrix.Data[7])
         + matrix.Data[6] * (matrix.Data[1] * matrix.Data[5] - matrix.Data[2] * matrix.Data[4]);
}

Math::FMatrix3 Math::FMatrix3::Transpose(const Math::FMatrix3& matrix)
{
    Math::FMatrix3 result;

    result.Data[0] = matrix.Data[0];
    result.Data[1] = matrix.Data[3];
    result.Data[2] = matrix.Data[6];

    result.Data[3] = matrix.Data[1];
    result.Data[4] = matrix.Data[4];
    result.Data[5] = matrix.Data[7];

    result.Data[6] = matrix.Data[2];
    result.Data[7] = matrix.Data[5];
    result.Data[8] = matrix.Data[8];

    return result;
}

Math::FMatrix3 Math::FMatrix3::Cofactor(const Math::FMatrix3& matrix)
{
    return FMatrix3(
         ((matrix.Data[4] * matrix.Data[8]) - (matrix.Data[5] * matrix.Data[7])), //0
        -((matrix.Data[3] * matrix.Data[8]) - (matrix.Data[5] * matrix.Data[6])), //1
         ((matrix.Data[3] * matrix.Data[7]) - (matrix.Data[4] * matrix.Data[6])), //2
        -((matrix.Data[1] * matrix.Data[8]) - (matrix.Data[2] * matrix.Data[7])), //3
         ((matrix.Data[0] * matrix.Data[8]) - (matrix.Data[2] * matrix.Data[6])), //4
        -((matrix.Data[0] * matrix.Data[7]) - (matrix.Data[1] * matrix.Data[6])), //5
         ((matrix.Data[1] * matrix.Data[5]) - (matrix.Data[2] * matrix.Data[4])), //6
        -((matrix.Data[0] * matrix.Data[5]) - (matrix.Data[2] * matrix.Data[3])), //7 
         ((matrix.Data[0] * matrix.Data[4]) - (matrix.Data[1] * matrix.Data[3]))); //8
}

Math::FMatrix3 Math::FMatrix3::Minor(const Math::FMatrix3& matrix)
{
    return FMatrix3(
        ((matrix.Data[4] * matrix.Data[8]) - (matrix.Data[5] * matrix.Data[7])), //0
        ((matrix.Data[3] * matrix.Data[8]) - (matrix.Data[5] * matrix.Data[6])), //1
        ((matrix.Data[3] * matrix.Data[7]) - (matrix.Data[4] * matrix.Data[6])), //2
        ((matrix.Data[1] * matrix.Data[8]) - (matrix.Data[2] * matrix.Data[7])), //3
        ((matrix.Data[0] * matrix.Data[8]) - (matrix.Data[2] * matrix.Data[6])), //4
        ((matrix.Data[0] * matrix.Data[7]) - (matrix.Data[1] * matrix.Data[6])), //5
        ((matrix.Data[1] * matrix.Data[5]) - (matrix.Data[2] * matrix.Data[4])), //6
        ((matrix.Data[0] * matrix.Data[5]) - (matrix.Data[2] * matrix.Data[3])), //7 
        ((matrix.Data[0] * matrix.Data[4]) - (matrix.Data[1] * matrix.Data[3]))); //8
}

Math::FMatrix3 Math::FMatrix3::Adjoint(const Math::FMatrix3& other)
{
    return Transpose(Cofactor(other));
}

Math::FMatrix3 Math::FMatrix3::Inverse(const Math::FMatrix3& matrix)
{
    const float determinant = Determinant(matrix);
    if (determinant == 0)
        throw std::logic_error("Division by 0");

    return Adjoint(matrix) / determinant;
}

Math::FMatrix3 Math::FMatrix3::Translation(const FVector2& translation)
{
    return FMatrix3(1, 0, translation.X,
                    0, 1, translation.Y,
                    0, 0, 1);
}

Math::FMatrix3 Math::FMatrix3::Translate(const Math::FMatrix3& matrix, const FVector2& translation)
{
    return matrix * Translation(translation);
}

Math::FMatrix3 Math::FMatrix3::Rotation(float rotation)
{
    return FMatrix3(std::cos(rotation), -std::sin(rotation), 0,
                    std::sin(rotation),  std::cos(rotation), 0,
                    0, 0, 1);
}

Math::FMatrix3 Math::FMatrix3::Rotate(const Math::FMatrix3& matrix, float rotation)
{
    return matrix * Rotation(rotation);
}

Math::FMatrix3 Math::FMatrix3::Scaling(const FVector2& scale)
{
    return FMatrix3(scale.X, 0, 0,
                    0, scale.Y, 0,
                    0, 0, 1);
}

Math::FMatrix3 Math::FMatrix3::Scale(const Math::FMatrix3& matrix, const FVector2& scale)
{
    return matrix * Scaling(scale);
}

Math::FVector3 Math::FMatrix3::GetRow(const Math::FMatrix3& matrix, uint8_t row)
{
    if (row >= 3)
        throw std::out_of_range("Invalid index : " + std::to_string(row) + " is out of range");

    return FVector3(matrix.Data[row * 3], matrix.Data[row * 3 + 1], matrix.Data[row * 3 + 2]);
}

Math::FVector3 Math::FMatrix3::GetColumn(const Math::FMatrix3& matrix, uint8_t column)
{
    if (column >= 3)
        throw std::out_of_range("Invalid index : " + std::to_string(column) + " is out of range");

    return FVector3(matrix.Data[column + 6], matrix.Data[column + 3], matrix.Data[column]);
}
