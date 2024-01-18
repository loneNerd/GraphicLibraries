#include "core/components/component_transform.hpp"

namespace Components = Engine::Core::Components;
namespace Math = Engine::Math;

Components::ComponentTransform::ComponentTransform(Math::FVector3 localPosition, Math::FQuaternion localRotation, Math::FVector3 localScale)
{
    m_transform.GenerateMatricesLocal(localPosition, localRotation, localScale);
}

std::string Components::ComponentTransform::GetName()
{
    return "Transform";
}

void Components::ComponentTransform::SetLocalPosition(Math::FVector3 newPosition)
{
    m_transform.SetLocalPosition(newPosition);
}

void Components::ComponentTransform::SetLocalRotation(Math::FQuaternion newRotation)
{
    m_transform.SetLocalRotation(newRotation);
}

void Components::ComponentTransform::SetLocalScale(Math::FVector3 newScale)
{
    m_transform.SetLocalScale(newScale);
}

void Components::ComponentTransform::SetWorldPosition(Math::FVector3 newPosition)
{
    m_transform.SetWorldPosition(newPosition);
}

void Components::ComponentTransform::SetWorldRotation(Math::FQuaternion newRotation)
{
    m_transform.SetWorldRotation(newRotation);
}

void Components::ComponentTransform::SetWorldScale(Math::FVector3 newScale)
{
    m_transform.SetWorldScale(newScale);
}

void Components::ComponentTransform::TranslateLocal(const Math::FVector3& translation)
{
    m_transform.TranslateLocal(translation);
}

void Components::ComponentTransform::RotateLocal(const Math::FQuaternion& rotation)
{
    m_transform.RotateLocal(rotation);
}

void Components::ComponentTransform::ScaleLocal(const Math::FVector3& scale)
{
    m_transform.ScaleLocal(scale);
}

const Math::FVector3& Components::ComponentTransform::GetLocalPosition() const
{
    return m_transform.GetLocalPosition();
}

const Math::FQuaternion& Components::ComponentTransform::GetLocalRotation() const
{
    return m_transform.GetLocalRotation();
}

const Math::FVector3& Components::ComponentTransform::GetLocalScale() const
{
    return m_transform.GetLocalScale();
}

const Math::FVector3& Components::ComponentTransform::GetWorldPosition() const
{
    return m_transform.GetWorldPosition();
}

const Math::FQuaternion& Components::ComponentTransform::GetWorldRotation() const
{
    return m_transform.GetWorldRotation();
}

const Math::FVector3& Components::ComponentTransform::GetWorldScale() const
{
    return m_transform.GetWorldScale();
}

const Math::FMatrix4& Components::ComponentTransform::GetLocalMatrix() const
{
    return m_transform.GetLocalMatrix();
}

const Math::FMatrix4& Components::ComponentTransform::GetWorldMatrix() const
{
    return m_transform.GetWorldMatrix();
}

Math::FTransform& Components::ComponentTransform::GetFTransform()
{
    return m_transform;
}

Math::FVector3 Components::ComponentTransform::GetWorldForward() const
{
    return m_transform.GetWorldForward();
}

Math::FVector3 Components::ComponentTransform::GetWorldUp() const
{
    return m_transform.GetWorldUp();
}

Math::FVector3 Components::ComponentTransform::GetWorldRight() const
{
    return m_transform.GetWorldRight();
}

Math::FVector3 Components::ComponentTransform::GetLocalForward() const
{
    return m_transform.GetLocalForward();
}

Math::FVector3 Components::ComponentTransform::GetLocalUp() const
{
    return m_transform.GetLocalUp();
}

Math::FVector3 Components::ComponentTransform::GetLocalRight() const
{
    return m_transform.GetLocalRight();
}
