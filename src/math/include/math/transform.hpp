#pragma once
#ifndef ENGINE_MATH_TRANSFORM_HPP_
#define ENGINE_MATH_TRANSFORM_HPP_

#include "matrix4.hpp"
#include "quaternion.hpp"
#include "transform_notifier.hpp"
#include "vector3.hpp"

namespace Engine::Math
{
    class FTransform
    {
    public:
        FTransform(FVector3 localPosition = FVector3(0.0f, 0.0f, 0.0f), FQuaternion localRotation = FQuaternion(0.0f, 0.0f, 0.0f, 1.0f), FVector3 localScale = FVector3(1.0f, 1.0f, 1.0f));
        ~FTransform();

        FTransform(const FTransform& other)             = delete;
        FTransform(FTransform&& other)                  = delete;
        FTransform& operator=(const FTransform& other)  = delete;
        FTransform& operator=(const FTransform&& other) = delete;

        void NotificationHandler(ETransformNotification notification);
        void SetParent(FTransform& parent);
        bool RemoveParent();
        bool HasParent() const;
        void GenerateMatricesWorld(FVector3 position, FQuaternion rotation, FVector3 scale);
        void GenerateMatricesLocal(FVector3 position, FQuaternion rotation, FVector3 scale);
        void UpdateWorldMatrix();
        void UpdateLocalMatrix();
        void SetLocalPosition(FVector3 newPosition);
        void SetLocalRotation(FQuaternion newRotation);
        void SetLocalScale(FVector3 newScale);
        void SetWorldPosition(FVector3 newPosition);
        void SetWorldRotation(FQuaternion newRotation);
        void SetWorldScale(FVector3 newScale);
        void TranslateLocal(const FVector3& translation);
        void RotateLocal(const FQuaternion& rotation);
        void ScaleLocal(const FVector3& scale);

        const FVector3& GetLocalPosition() const;
        const FQuaternion& GetLocalRotation() const;
        const FVector3& GetLocalScale() const;
        const FVector3& GetWorldPosition() const;
        const FQuaternion& GetWorldRotation() const;
        const FVector3& GetWorldScale() const;
        const FMatrix4& GetLocalMatrix() const;
        const FMatrix4& GetWorldMatrix() const;

        FVector3 GetWorldForward() const;
        FVector3 GetWorldUp() const;
        FVector3 GetWorldRight() const;
        FVector3 GetLocalForward() const;
        FVector3 GetLocalUp() const;
        FVector3 GetLocalRight() const;

        TransformNotifier Notifier;
        TransformNotifier::NotificationHandlerID NotificationHandlerID;

    private:
        void preDecomposeWorldMatrix();
        void preDecomposeLocalMatrix();

        FVector3 m_localPosition;
        FQuaternion m_localRotation;
        FVector3 m_localScale;
        FVector3 m_worldPosition;
        FQuaternion m_worldRotation;
        FVector3 m_worldScale;

        FMatrix4 m_localMatrix;
        FMatrix4 m_worldMatrix;

        FTransform* m_parent;
    };
}

#endif // ENGINE_MATH_TRANSFORM_HPP_
