#pragma once
#ifndef ENGINE_CORE_COMPONENTS_COMPONENT_TRANSFORM_HPP_
#define ENGINE_CORE_COMPONENTS_COMPONENT_TRANSFORM_HPP_

#include "math/quaternion.hpp"
#include "math/transform.hpp"
#include "math/vector3.hpp"

#include "component.hpp"

namespace Engine
{
namespace Core
{

class Actor;

namespace Components
{
    class ComponentTransform : public Component
    {
    public:
        ComponentTransform(struct Math::FVector3 localPosition = Math::FVector3(0.0f, 0.0f, 0.0f), Math::FQuaternion localRotation = Math::FQuaternion::Identity, struct Math::FVector3 localScale = Math::FVector3(1.0f, 1.0f, 1.0f));

        std::string GetName() override;
        void SetLocalPosition(struct Math::FVector3 newPosition);
        void SetLocalRotation(Math::FQuaternion newRotation);
        void SetLocalScale(struct Math::FVector3 newScale);
        void SetWorldPosition(struct Math::FVector3 newPosition);
        void SetWorldRotation(Math::FQuaternion newRotation);
        void SetWorldScale(struct Math::FVector3 newScale);
        void TranslateLocal(const struct Math::FVector3& translation);
        void RotateLocal(const Math::FQuaternion& rotation);
        void ScaleLocal(const struct Math::FVector3& scale);
        const Math::FVector3& GetLocalPosition() const;
        const Math::FQuaternion& GetLocalRotation() const;
        const Math::FVector3& GetLocalScale() const;
        const Math::FVector3& GetWorldPosition() const;
        const Math::FQuaternion& GetWorldRotation() const;
        const Math::FVector3& GetWorldScale() const;
        const Math::FMatrix4& GetLocalMatrix() const;
        const Math::FMatrix4& GetWorldMatrix() const;
        Math::FTransform& GetFTransform();
        Math::FVector3 GetWorldForward() const;
        Math::FVector3 GetWorldUp() const;
        Math::FVector3 GetWorldRight() const;
        Math::FVector3 GetLocalForward() const;
        Math::FVector3 GetLocalUp() const;
        Math::FVector3 GetLocalRight() const;

    private:
        Math::FTransform m_transform;
    };
}
}
}

#endif // ENGINE_CORE_COMPONENTS_COMPONENT_TRANSFORM_HPP_
