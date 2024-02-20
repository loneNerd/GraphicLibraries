#include "math/matrix4.hpp"

#include <cmath>
#include <stdexcept>
#include <string>

#include "math/quaternion.hpp"

namespace Math = Engine::Math;

#define PI 3.14159265359f
#define EPSILON 0.00001f

Math::FMatrix4::FMatrix4()
{
    memcpy(Data.data(), Math::FMatrix4::GetIdentity().Data.data(), Data.size() * sizeof(float));
}

Math::FMatrix4::FMatrix4(float element1, float element2, float element3, float element4, float element5, float element6, float element7, float element8, float element9, float element10, float element11, float element12, float element13, float element14, float element15, float element16)
{
    Data[0]  = element1;
    Data[1]  = element2;
    Data[2]  = element3;
    Data[3]  = element4;
    Data[4]  = element5;
    Data[5]  = element6;
    Data[6]  = element7;
    Data[7]  = element8;
    Data[8]  = element9;
    Data[9]  = element10;
    Data[10] = element11;
    Data[11] = element12;
    Data[12] = element13;
    Data[13] = element14;
    Data[14] = element15;
    Data[15] = element16;
}

Math::FMatrix4::FMatrix4(const Math::FMatrix4& other)
{
    *this = other;
}

Math::FMatrix4& Math::FMatrix4::operator=(const Math::FMatrix4& other)
{
    memcpy(Data.data(), other.Data.data(), Data.size() * sizeof(float));
    return *this;
}

bool Math::FMatrix4::operator==(const Math::FMatrix4& other)
{
    return AreEquals(*this, other);
}

Math::FMatrix4 Math::FMatrix4::operator+(const Math::FMatrix4& other) const
{
    return Add(*this, other);
}

Math::FMatrix4& Math::FMatrix4::operator+=(const Math::FMatrix4& other)
{
    *this = Add(*this, other);
    return *this;
}

Math::FMatrix4 Math::FMatrix4::operator-(float scalar) const
{
    return Subtract(*this, scalar);
}

Math::FMatrix4 Math::FMatrix4::operator-(const Math::FMatrix4& other) const
{
    return Subtract(*this, other);
}

Math::FMatrix4& Math::FMatrix4::operator-=(const Math::FMatrix4& other)
{
    *this = Subtract(*this, other);
    return *this;
}

Math::FMatrix4 Math::FMatrix4::operator*(float scalar) const
{
    return Multiply(*this, scalar);
}

Math::FMatrix4& Math::FMatrix4::operator*=(float scalar)
{
    *this = Multiply(*this, scalar);
    return *this;
}

Math::FVector4 Math::FMatrix4::operator*(const FVector4& vector) const
{
    return Multiply(*this, vector);
}

Math::FMatrix4 Math::FMatrix4::operator*(const Math::FMatrix4& other) const
{
    return Multiply(*this, other);
}

Math::FMatrix4& Math::FMatrix4::operator*=(const Math::FMatrix4& other)
{
    *this = Multiply(*this, other);
    return *this;
}

Math::FMatrix4 Math::FMatrix4::operator/(float scalar) const
{
    return Divide(*this, scalar);
}

Math::FMatrix4& Math::FMatrix4::operator/=(float scalar)
{
    *this = Divide(*this, scalar);
    return *this;
}

Math::FMatrix4 Math::FMatrix4::operator/(const Math::FMatrix4& other) const
{
    return Divide(*this, other);
}

Math::FMatrix4& Math::FMatrix4::operator/=(const Math::FMatrix4& other)
{
    *this = Divide(*this, other);
    return *this;
}

float& Math::FMatrix4::operator()(uint8_t row, uint8_t column)
{
    if (row >= 4 || column >= 4)
        throw std::out_of_range(
            "Invalid index : " + std::to_string(row) + "," + std::to_string(column) + " is out of range");
    return Data[4 * row + column];
}

bool Math::FMatrix4::AreEquals(const Math::FMatrix4& left, const Math::FMatrix4& right)
{
    return memcmp(&left, &right, 16 * sizeof(float)) == 0;
}

Math::FMatrix4 Math::FMatrix4::Add(const Math::FMatrix4& left, float scalar)
{
    Math::FMatrix4 result(left);
    for (int8_t i = 0; i < 16; i++)
        result.Data[i] += scalar;
    return result;
}

Math::FMatrix4 Math::FMatrix4::Add(const Math::FMatrix4& left, const Math::FMatrix4& right)
{
    Math::FMatrix4 result(left);
    for (int8_t i = 0; i < 16; i++)
        result.Data[i] += right.Data[i];
    return result;
}

Math::FMatrix4 Math::FMatrix4::Subtract(const Math::FMatrix4& left, float scalar)
{
    Math::FMatrix4 result(left);
    for (int8_t i = 0; i < 16; ++i)
        result.Data[i] -= scalar;
    return result;
}

Math::FMatrix4 Math::FMatrix4::Subtract(const Math::FMatrix4& left, const Math::FMatrix4& right)
{
    Math::FMatrix4 result(left);
    for (int8_t i = 0; i < 16; ++i)
        result.Data[i] -= right.Data[i];
    return result;
}

Math::FMatrix4 Math::FMatrix4::Multiply(const Math::FMatrix4& left, float scalar)
{
    Math::FMatrix4 result(left);
    for (int8_t i = 0; i < 16; ++i)
        result.Data[i] *= scalar;
    return result;
}

Math::FVector4 Math::FMatrix4::Multiply(const Math::FMatrix4& matrix, const FVector4& vector)
{
    Math::FVector4 multiply;

    multiply.X = ((matrix.Data[0]  * vector.X) + (matrix.Data[1]  * vector.Y) + 
                  (matrix.Data[2]  * vector.Z) + (matrix.Data[3]  * vector.W));
    multiply.Y = ((matrix.Data[4]  * vector.X) + (matrix.Data[5]  * vector.Y) + 
                  (matrix.Data[6]  * vector.Z) + (matrix.Data[7]  * vector.W));
    multiply.Z = ((matrix.Data[8]  * vector.X) + (matrix.Data[9]  * vector.Y) + 
                  (matrix.Data[10] * vector.Z) + (matrix.Data[11] * vector.W));
    multiply.W = ((matrix.Data[12] * vector.X) + (matrix.Data[13] * vector.Y) + 
                  (matrix.Data[14] * vector.Z) + (matrix.Data[15] * vector.W));
    return multiply;
}

Math::FMatrix4 Math::FMatrix4::Multiply(const Math::FMatrix4& left, const Math::FMatrix4& right)
{
    return FMatrix4(
        ((left.Data[0] * right.Data[0]) + (left.Data[1] * right.Data[4]) + (left.Data[
            2] * right.Data[8]) + (left.Data[3] * right.Data[12])),
        ((left.Data[0] * right.Data[1]) + (left.Data[1] * right.Data[5]) + (left.Data[
            2] * right.Data[9]) + (left.Data[3] * right.Data[13])),
                ((left.Data[0] * right.Data[2]) + (left.Data[1] * right.Data[6]) + (left.Data[
                    2] * right.Data[10]) + (left.Data[3] * right.Data[14])),
                ((left.Data[0] * right.Data[3]) + (left.Data[1] * right.Data[7]) + (left.Data[
                    2] * right.Data[11]) + (left.Data[3] * right.Data[15])),

                        ((left.Data[4] * right.Data[0]) + (left.Data[5] * right.Data[4]) + (left.Data[
                            6] * right.Data[8]) + (left.Data[7] * right.Data[12])),
                        ((left.Data[4] * right.Data[1]) + (left.Data[5] * right.Data[5]) + (left.Data[
                            6] * right.Data[9]) + (left.Data[7] * right.Data[13])),
                                ((left.Data[4] * right.Data[2]) + (left.Data[5] * right.Data[6]) + (left.Data[
                                    6] * right.Data[10]) + (left.Data[7] * right.Data[14])),
                                ((left.Data[4] * right.Data[3]) + (left.Data[5] * right.Data[7]) + (left.Data[
                                    6] * right.Data[11]) + (left.Data[7] * right.Data[15])),

                                        ((left.Data[8] * right.Data[0]) + (left.Data[9] * right.Data[4]) + (left.Data[
                                            10] * right.Data[8]) + (left.Data[11] * right.Data[12])),
                                        ((left.Data[8] * right.Data[1]) + (left.Data[9] * right.Data[5]) + (left.Data[
                                            10] * right.Data[9]) + (left.Data[11] * right.Data[13])),
                                                ((left.Data[8] * right.Data[2]) + (left.Data[9] * right.Data[6]) + (left.Data[
                                                    10] * right.Data[10]) + (left.Data[11] * right.Data[14])),
                                                ((left.Data[8] * right.Data[3]) + (left.Data[9] * right.Data[7]) + (left.Data[
                                                    10] * right.Data[11]) + (left.Data[11] * right.Data[15])),

                                                        ((left.Data[12] * right.Data[0]) + (left.Data[13] * right.Data[4]) + (left.
                                                            Data[14] * right.Data[8]) + (left.Data[15] * right.Data[12])),
                                                        ((left.Data[12] * right.Data[1]) + (left.Data[13] * right.Data[5]) + (left.
                                                            Data[14] * right.Data[9]) + (left.Data[15] * right.Data[13])),
                                                        ((left.Data[12] * right.Data[2]) + (left.Data[13] * right.Data[6]) + (left.
                                                            Data[14] * right.Data[10]) + (left.Data[15] * right.Data[14])),
                                                        ((left.Data[12] * right.Data[3]) + (left.Data[13] * right.Data[7]) + (left.
                                                            Data[14] * right.Data[11]) + (left.Data[15] * right.Data[15])));
}

Math::FMatrix4 Math::FMatrix4::Divide(const Math::FMatrix4& left, float scalar)
{
    Math::FMatrix4 result(left);
    for (float& element : result.Data)
        element /= scalar;

    return result;
}

Math::FMatrix4 Math::FMatrix4::Divide(const Math::FMatrix4& left, const Math::FMatrix4& right)
{
    Math::FMatrix4 leftCopy(left);
    return leftCopy * Inverse(right);
}

bool Math::FMatrix4::IsIdentity(const Math::FMatrix4& matrix)
{
    return memcmp(FMatrix4::GetIdentity().Data.data(), matrix.Data.data(), FMatrix4::GetIdentity().Data.size() * sizeof(float)) == 0;
}

float Math::FMatrix4::GetMinor(float minor0, float minor1, float minor2, float minor3, float minor4, float minor5, float minor6, float minor7, float minor8)
{
    return minor0 * (minor4 * minor8 - minor5 * minor7)
         - minor1 * (minor3 * minor8 - minor5 * minor6)
         + minor2 * (minor3 * minor7 - minor4 * minor6);
}

float Math::FMatrix4::Determinant(const Math::FMatrix4& matrix)
{
    return matrix.Data[0] * GetMinor(matrix.Data[5], matrix.Data[9], matrix.Data[13], matrix.Data[6], matrix.Data[10], matrix.Data[14],
        matrix.Data[7], matrix.Data[11], matrix.Data[15])
        - matrix.Data[4] * GetMinor(matrix.Data[1], matrix.Data[9], matrix.Data[13], matrix.Data[2], matrix.Data[10], matrix.Data[14],
            matrix.Data[3], matrix.Data[11], matrix.Data[15])
        + matrix.Data[8] * GetMinor(matrix.Data[1], matrix.Data[5], matrix.Data[13], matrix.Data[2], matrix.Data[6], matrix.Data[14],
            matrix.Data[3], matrix.Data[7], matrix.Data[15])
        - matrix.Data[12] * GetMinor(matrix.Data[1], matrix.Data[5], matrix.Data[9], matrix.Data[2], matrix.Data[6], matrix.Data[10],
            matrix.Data[3], matrix.Data[7], matrix.Data[11]);
}

Math::FMatrix4 Math::FMatrix4::Transpose(const Math::FMatrix4& matrix)
{
    Math::FMatrix4 TransposedMatrix(matrix);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            TransposedMatrix.Data[4 * j + i] = matrix.Data[4 * i + j];
        }
    }
    return TransposedMatrix;
}

Math::FMatrix4 Math::FMatrix4::Inverse(const Math::FMatrix4& matrix)
{
    const float determinant = Determinant(matrix);
    if (determinant == 0)
        throw std::logic_error("Division by 0");

    const float cof0 = GetMinor(matrix.Data[5], matrix.Data[9],  matrix.Data[13],
                                matrix.Data[6], matrix.Data[10], matrix.Data[14],
                                matrix.Data[7], matrix.Data[11], matrix.Data[15]);

    const float cof1 = GetMinor(matrix.Data[1], matrix.Data[9],  matrix.Data[13],
                                matrix.Data[2], matrix.Data[10], matrix.Data[14],
                                matrix.Data[3], matrix.Data[11], matrix.Data[15]);

    const float cof2 = GetMinor(matrix.Data[1], matrix.Data[5], matrix.Data[13],
                                matrix.Data[2], matrix.Data[6], matrix.Data[14],
                                matrix.Data[3], matrix.Data[7], matrix.Data[15]);

    const float cof3 = GetMinor(matrix.Data[1], matrix.Data[5], matrix.Data[9],
                                matrix.Data[2], matrix.Data[6], matrix.Data[10],
                                matrix.Data[3], matrix.Data[7], matrix.Data[11]);

    const float det = matrix.Data[0] * cof0 - matrix.Data[4] * cof1 + matrix.Data[8] * cof2 - matrix.Data[12] * cof3;

    if (fabs(det) <= EPSILON)
        return FMatrix4::GetIdentity();

    const float cof4 = GetMinor(matrix.Data[4], matrix.Data[8],  matrix.Data[12],
                                matrix.Data[6], matrix.Data[10], matrix.Data[14],
                                matrix.Data[7], matrix.Data[11], matrix.Data[15]);

    const float cof5 = GetMinor(matrix.Data[0], matrix.Data[8],  matrix.Data[12],
                                matrix.Data[2], matrix.Data[10], matrix.Data[14],
                                matrix.Data[3], matrix.Data[11], matrix.Data[15]);

    const float cof6 = GetMinor(matrix.Data[0], matrix.Data[4], matrix.Data[12],
                                matrix.Data[2], matrix.Data[6], matrix.Data[14],
                                matrix.Data[3], matrix.Data[7], matrix.Data[15]);

    const float cof7 = GetMinor(matrix.Data[0], matrix.Data[4], matrix.Data[8],
                                matrix.Data[2], matrix.Data[6], matrix.Data[10],
                                matrix.Data[3], matrix.Data[7], matrix.Data[11]);

    const float cof8 = GetMinor(matrix.Data[4], matrix.Data[8],  matrix.Data[12],
                                matrix.Data[5], matrix.Data[9],  matrix.Data[13],
                                matrix.Data[7], matrix.Data[11], matrix.Data[15]);

    const float cof9 = GetMinor(matrix.Data[0], matrix.Data[8],  matrix.Data[12],
                                matrix.Data[1], matrix.Data[9],  matrix.Data[13],
                                matrix.Data[3], matrix.Data[11], matrix.Data[15]);

    const float cof10 = GetMinor(matrix.Data[0], matrix.Data[4], matrix.Data[12],
                                 matrix.Data[1], matrix.Data[5], matrix.Data[13],
                                 matrix.Data[3], matrix.Data[7], matrix.Data[15]);

    const float cof11 = GetMinor(matrix.Data[0], matrix.Data[4], matrix.Data[8],
                                 matrix.Data[1], matrix.Data[5], matrix.Data[9],
                                 matrix.Data[3], matrix.Data[7], matrix.Data[11]);

    const float cof12 = GetMinor(matrix.Data[4], matrix.Data[8],  matrix.Data[12],
                                 matrix.Data[5], matrix.Data[9],  matrix.Data[13],
                                 matrix.Data[6], matrix.Data[10], matrix.Data[14]);

    const float cof13 = GetMinor(matrix.Data[0], matrix.Data[8],  matrix.Data[12],
                                 matrix.Data[1], matrix.Data[9],  matrix.Data[13],
                                 matrix.Data[2], matrix.Data[10], matrix.Data[14]);

    const float cof14 = GetMinor(matrix.Data[0], matrix.Data[4], matrix.Data[12],
                                 matrix.Data[1], matrix.Data[5], matrix.Data[13],
                                 matrix.Data[2], matrix.Data[6], matrix.Data[14]);

    const float cof15 = GetMinor(matrix.Data[0], matrix.Data[4], matrix.Data[8],
                                 matrix.Data[1], matrix.Data[5], matrix.Data[9],
                                 matrix.Data[2], matrix.Data[6], matrix.Data[10]);

    const float detInv = 1.0f / det;
    Math::FMatrix4 inverse;

    inverse.Data[0]  =  detInv * cof0;
    inverse.Data[4]  = -detInv * cof4;
    inverse.Data[8]  =  detInv * cof8;
    inverse.Data[12] = -detInv * cof12;
    inverse.Data[1]  = -detInv * cof1;
    inverse.Data[5]  =  detInv * cof5;
    inverse.Data[9]  = -detInv * cof9;
    inverse.Data[13] =  detInv * cof13;
    inverse.Data[2]  =  detInv * cof2;
    inverse.Data[6]  = -detInv * cof6;
    inverse.Data[10] =  detInv * cof10;
    inverse.Data[14] = -detInv * cof14;
    inverse.Data[3]  = -detInv * cof3;
    inverse.Data[7]  =  detInv * cof7;
    inverse.Data[11] = -detInv * cof11;
    inverse.Data[15] =  detInv * cof15;

    return inverse;
}

Math::FMatrix4 Math::FMatrix4::Translation(const FVector3& translation)
{
    return FMatrix4(1, 0, 0, translation.X,
                    0, 1, 0, translation.Y,
                    0, 0, 1, translation.Z,
                    0, 0, 0, 1);
}

Math::FMatrix4 Math::FMatrix4::Translate(const Math::FMatrix4& matrix, const FVector3& translation)
{
    return matrix * Translation(translation);
}

Math::FMatrix4 Math::FMatrix4::RotationOnAxisX(float rotation)
{
    return FMatrix4(1, 0, 0, 0,
        0, std::cos(rotation), -std::sin(rotation), 0,
        0, std::sin(rotation), std::cos(rotation), 0,
        0, 0, 0, 1);
}

Math::FMatrix4 Math::FMatrix4::RotateOnAxisX(const Math::FMatrix4& matrix, float rotation)
{
    return matrix * RotationOnAxisX(rotation);
}

Math::FMatrix4 Math::FMatrix4::RotationOnAxisY(float rotation)
{
    return FMatrix4(std::cos(rotation), 0, -std::sin(rotation), 0,
        0, 1, 0, 0,
        std::sin(rotation), 0, std::cos(rotation), 0,
        0, 0, 0, 1);
}

Math::FMatrix4 Math::FMatrix4::RotateOnAxisY(const Math::FMatrix4& matrix, float rotation)
{
    return matrix * RotationOnAxisY(rotation);
}

Math::FMatrix4 Math::FMatrix4::RotationOnAxisZ(float rotation)
{
    return FMatrix4(std::cos(rotation), -std::sin(rotation), 0, 0,
                    std::sin(rotation),  std::cos(rotation), 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1);
}

Math::FMatrix4 Math::FMatrix4::RotateOnAxisZ(const Math::FMatrix4& matrix, float rotation)
{
    return matrix * RotationOnAxisZ(rotation);
}

Math::FMatrix4 Math::FMatrix4::RotationYXZ(float rotation)
{
    Math::FMatrix4 Xrot = RotationOnAxisX(rotation);
    Math::FMatrix4 Yrot = RotationOnAxisY(rotation);
    Math::FMatrix4 Zrot = RotationOnAxisZ(rotation);

    return Yrot * Xrot * Zrot;
}

Math::FMatrix4 Math::FMatrix4::RotateYXZ(const Math::FMatrix4& matrix, float rotation)
{
    return matrix * RotationYXZ(rotation);
}

Math::FMatrix4 Math::FMatrix4::Scaling(const FVector3& scale)
{
    return FMatrix4(scale.X, 0, 0, 0,
                    0, scale.Y, 0, 0,
                    0, 0, scale.Z, 0,
                    0, 0, 0, 1);
}

Math::FMatrix4 Math::FMatrix4::Scale(const Math::FMatrix4& matrix, const FVector3& scale)
{
    return matrix * Scaling(scale);
}

Math::FMatrix4 Math::FMatrix4::Rotation(const FQuaternion& quaternion)
{
    return FQuaternion::ToMatrix4(quaternion);
}

Math::FMatrix4 Math::FMatrix4::Rotate(const Math::FMatrix4& matrix, const FQuaternion& quaternion)
{
    return matrix * Rotation(quaternion);
}

Math::FMatrix4 Math::FMatrix4::CreatePerspective(const float fov, const float aspectRatio, const float zNear, const float zFar)
{
    const float tangent = tanf(fov / 2.0f * PI / 180.0f);
    const float height = zNear * tangent;
    const float width = height * aspectRatio;

    return CreateFrustum(-width, width, -height, height, zNear, zFar);
}

Math::FMatrix4 Math::FMatrix4::CreateOrthographic(const float size, const float aspectRatio, const float zNear, const float zFar)
{
    auto ortho = Math::FMatrix4::GetIdentity();

    const auto right = size * aspectRatio;
    const auto left = -right;

    const auto top = size;
    const auto bottom = -top;

    ortho(0, 0) = 2.0f / (right - left);
    ortho(1, 1) = 2.0f / (top - bottom);
    ortho(2, 2) = -2.0f / (zFar - zNear);
    ortho(0, 3) = -(right + left) / (right - left);
    ortho(1, 3) = -(top + bottom) / (top - bottom);
    ortho(2, 3) = -(zFar + zNear) / (zFar - zNear);
    ortho(3, 3) = 1.0f;

    return ortho;
}

Math::FMatrix4 Math::FMatrix4::CreateView(const float eyeX, const float eyeY, const float eyeZ, const float lookX, const float lookY, const float lookZ, const float upX, const float upY, const float upZ)
{
    const FVector3 eye(eyeX, eyeY, eyeZ);
    const FVector3 look(lookX, lookY, lookZ);
    const FVector3 up(upX, upY, upZ);

    const FVector3 forward(eye - look);
    FVector3::Normalize(forward);

    const FVector3 upXForward(FVector3::Cross(up, forward));
    FVector3::Normalize(upXForward);

    const FVector3 v(FVector3::Cross(forward, upXForward));

    Math::FMatrix4 View;

    View.Data[0] = upXForward.X;
    View.Data[1] = upXForward.Y;
    View.Data[2] = upXForward.Z;
    View.Data[3] = -FVector3::Dot(eye, upXForward);

    View.Data[4] = v.X;
    View.Data[5] = v.Y;
    View.Data[6] = v.Z;
    View.Data[7] = -FVector3::Dot(eye, v);

    View.Data[8]  = forward.X;
    View.Data[9]  = forward.Y;
    View.Data[10] = forward.Z;
    View.Data[11] = -FVector3::Dot(eye, forward);

    return View;
}

Math::FMatrix4 Math::FMatrix4::CreateFrustum(const float left, const float right, const float bottom, const float top, const float zNear, const float zFar)
{
    const float maxView = 2.0f * zNear;
    const float width   = right - left;
    const float height  = top - bottom;
    const float zRange  = zFar - zNear;

    Math::FMatrix4 Frustum;

    Frustum.Data[0]  = maxView / width;
    Frustum.Data[5]  = maxView / height;
    Frustum.Data[2]  = (right + left) / width;
    Frustum.Data[6]  = (top + bottom) / height;
    Frustum.Data[10] = (-zFar - zNear) / zRange;
    Frustum.Data[14] = -1.0f;
    Frustum.Data[11] = (-maxView * zFar) / zRange;
    Frustum.Data[15] = 0.0f;

    return Frustum;
}

Math::FVector4 Math::FMatrix4::GetRow(const Math::FMatrix4& matrix, uint8_t row)
{
    if (row >= 4)
        throw std::out_of_range("Invalid index : " + std::to_string(row) + " is out of range");

    return FVector4(matrix.Data[row * 4], matrix.Data[row * 4 + 1], matrix.Data[row * 4 + 2], matrix.Data[row * 4 + 3]);
}

Math::FVector4 Math::FMatrix4::GetColumn(const Math::FMatrix4& matrix, uint8_t column)
{
    if (column >= 4)
        throw std::out_of_range("Invalid index : " + std::to_string(column) + " is out of range");

    return FVector4(matrix.Data[column], matrix.Data[column + 4], matrix.Data[column + 8], matrix.Data[column + 12]);
}
