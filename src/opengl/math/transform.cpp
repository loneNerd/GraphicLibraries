#include "transform.hpp"

namespace Math = Engine::Math;

Math::FTransform::FTransform(FVector3 localPosition, FQuaternion localRotation, FVector3 localScale) :
    NotificationHandlerID(-1),
    m_parent(nullptr)
{
    GenerateMatricesLocal(localPosition, localRotation, localScale);
}

Math::FTransform::~FTransform()
{
    Notifier.NotifyChildren(ETransformNotification::Destroyed);
}

void Math::FTransform::NotificationHandler(ETransformNotification notification)
{
    switch (notification)
    {
        case ETransformNotification::Changed:
        {
            UpdateWorldMatrix();
            break;
        }
        case ETransformNotification::Destroyed:
        {
            /*
            * RemoveParent() is not called here because it is unsafe to remove a notification handler
            * while the parent is iterating on his notification handlers (Segfault otherwise)
            */
            GenerateMatricesLocal(m_worldPosition, m_worldRotation, m_worldScale);
            m_parent = nullptr;
            UpdateWorldMatrix();
            break;
        }
    }
}

void Math::FTransform::SetParent(FTransform& parent)
{
    m_parent = &parent;

    NotificationHandlerID = m_parent->Notifier.AddNotificationHandler(std::bind(&Math::FTransform::NotificationHandler, this, std::placeholders::_1));

    UpdateWorldMatrix();
}

bool Math::FTransform::RemoveParent()
{
    if (m_parent != nullptr)
    {
        m_parent->Notifier.RemoveNotificationHandler(NotificationHandlerID);
        m_parent = nullptr;
        UpdateWorldMatrix();

        return true;
    }

    return false;
}

bool Math::FTransform::HasParent() const
{
    return m_parent != nullptr;
}

void Math::FTransform::GenerateMatricesLocal(FVector3 position, FQuaternion rotation, FVector3 scale)
{
    m_localMatrix = FMatrix4::Translation(position) * FQuaternion::ToMatrix4(FQuaternion::Normalize(rotation)) * FMatrix4::Scaling(scale);
    m_localPosition = position;
    m_localRotation = rotation;
    m_localScale = scale;

    UpdateWorldMatrix();
}

void Math::FTransform::GenerateMatricesWorld(FVector3 position, FQuaternion rotation, FVector3 scale)
{
    m_worldMatrix = FMatrix4::Translation(position) * FQuaternion::ToMatrix4(FQuaternion::Normalize(rotation)) * FMatrix4::Scaling(scale);
    m_worldPosition = position;
    m_worldRotation = rotation;
    m_worldScale = scale;

    UpdateLocalMatrix();
}

void Math::FTransform::UpdateWorldMatrix()
{
    m_worldMatrix = HasParent() ? m_parent->m_worldMatrix * m_localMatrix : m_localMatrix;
    preDecomposeWorldMatrix();

    Notifier.NotifyChildren(ETransformNotification::Changed);
}

void Math::FTransform::UpdateLocalMatrix()
{
    m_localMatrix = HasParent() ? FMatrix4::Inverse(m_parent->m_worldMatrix) * m_worldMatrix : m_worldMatrix;
    preDecomposeLocalMatrix();

    Notifier.NotifyChildren(ETransformNotification::Changed);
}

void Math::FTransform::SetLocalPosition(FVector3 newPosition)
{
    GenerateMatricesLocal(newPosition, m_localRotation, m_localScale);
}

void Math::FTransform::SetLocalRotation(FQuaternion newRotation)
{
    GenerateMatricesLocal(m_localPosition, newRotation, m_localScale);
}

void Math::FTransform::SetLocalScale(FVector3 newScale)
{
    GenerateMatricesLocal(m_localPosition, m_localRotation, newScale);
}

void Math::FTransform::SetWorldPosition(FVector3 newPosition)
{
    GenerateMatricesWorld(newPosition, m_worldRotation, m_worldScale);
}

void Math::FTransform::SetWorldRotation(FQuaternion newRotation)
{
    GenerateMatricesWorld(m_worldPosition, newRotation, m_worldScale);
}

void Math::FTransform::SetWorldScale(FVector3 newScale)
{
    GenerateMatricesWorld(m_worldPosition, m_worldRotation, newScale);
}

void Math::FTransform::TranslateLocal(const FVector3& translation)
{
    SetLocalPosition(m_localPosition + translation);
}

void Math::FTransform::RotateLocal(const FQuaternion& rotation)
{
    SetLocalRotation(m_localRotation * rotation);
}

void Math::FTransform::ScaleLocal(const FVector3& scale)
{
    SetLocalScale(FVector3
    (
        m_localScale.X * scale.X,
        m_localScale.Y * scale.Y,
        m_localScale.Z * scale.Z
    ));
}

const Math::FVector3& Math::FTransform::GetLocalPosition() const
{
    return m_localPosition;
}

const Math::FQuaternion& Math::FTransform::GetLocalRotation() const
{
    return m_localRotation;
}

const Math::FVector3& Math::FTransform::GetLocalScale() const
{
    return m_localScale;
}

const Math::FVector3& Math::FTransform::GetWorldPosition() const
{
    return m_worldPosition;
}

const Math::FQuaternion& Math::FTransform::GetWorldRotation() const
{
    return m_worldRotation;
}

const Math::FVector3& Math::FTransform::GetWorldScale() const
{
    return m_worldScale;
}

const Math::FMatrix4& Math::FTransform::GetLocalMatrix() const
{
    return m_localMatrix;
}

const Math::FMatrix4& Math::FTransform::GetWorldMatrix() const
{
    return m_worldMatrix;
}

Math::FVector3 Math::FTransform::GetWorldForward() const
{
    return m_worldRotation * FVector3::Forward;
}

Math::FVector3 Math::FTransform::GetWorldUp() const
{
    return m_worldRotation * FVector3::Up;
}

Math::FVector3 Math::FTransform::GetWorldRight() const
{
    return m_worldRotation * FVector3::Right;
}

Math::FVector3 Math::FTransform::GetLocalForward() const
{
    return m_localRotation * FVector3::Forward;
}

Math::FVector3 Math::FTransform::GetLocalUp() const
{
    return m_localRotation * FVector3::Up;
}

Math::FVector3 Math::FTransform::GetLocalRight() const
{
    return m_localRotation * FVector3::Right;
}

void Math::FTransform::preDecomposeWorldMatrix()
{
    m_worldPosition.X = m_worldMatrix(0, 3);
    m_worldPosition.Y = m_worldMatrix(1, 3);
    m_worldPosition.Z = m_worldMatrix(2, 3);

    FVector3 columns[3] =
    {
        { m_worldMatrix(0, 0), m_worldMatrix(1, 0), m_worldMatrix(2, 0)},
        { m_worldMatrix(0, 1), m_worldMatrix(1, 1), m_worldMatrix(2, 1)},
        { m_worldMatrix(0, 2), m_worldMatrix(1, 2), m_worldMatrix(2, 2)},
    };

    m_worldScale.X = FVector3::Length(columns[0]);
    m_worldScale.Y = FVector3::Length(columns[1]);
    m_worldScale.Z = FVector3::Length(columns[2]);

    if (m_worldScale.X)
    {
        columns[0] /= m_worldScale.X;
    }

    if (m_worldScale.Y)
    {
        columns[1] /= m_worldScale.Y;
    }

    if (m_worldScale.Z)
    {
        columns[2] /= m_worldScale.Z;
    }

    FMatrix3 rotationMatrix
    (
        columns[0].X, columns[1].X, columns[2].X,
        columns[0].Y, columns[1].Y, columns[2].Y,
        columns[0].Z, columns[1].Z, columns[2].Z
    );

    m_worldRotation = FQuaternion(rotationMatrix);
}

void Math::FTransform::preDecomposeLocalMatrix()
{
    m_localPosition.X = m_localMatrix(0, 3);
    m_localPosition.Y = m_localMatrix(1, 3);
    m_localPosition.Z = m_localMatrix(2, 3);

    FVector3 columns[3] =
    {
        { m_localMatrix(0, 0), m_localMatrix(1, 0), m_localMatrix(2, 0)},
        { m_localMatrix(0, 1), m_localMatrix(1, 1), m_localMatrix(2, 1)},
        { m_localMatrix(0, 2), m_localMatrix(1, 2), m_localMatrix(2, 2)},
    };

    m_localScale.X = FVector3::Length(columns[0]);
    m_localScale.Y = FVector3::Length(columns[1]);
    m_localScale.Z = FVector3::Length(columns[2]);

    if (m_localScale.X)
    {
        columns[0] /= m_localScale.X;
    }
    if (m_localScale.Y)
    {
        columns[1] /= m_localScale.Y;
    }
    if (m_localScale.Z)
    {
        columns[2] /= m_localScale.Z;
    }

    FMatrix3 rotationMatrix
    (
        columns[0].X, columns[1].X, columns[2].X,
        columns[0].Y, columns[1].Y, columns[2].Y,
        columns[0].Z, columns[1].Z, columns[2].Z
    );

    m_localRotation = FQuaternion(rotationMatrix);
}
