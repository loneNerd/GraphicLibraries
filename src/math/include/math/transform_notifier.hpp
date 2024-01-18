#pragma once
#ifndef ENGINE_MATH_TRANSFORM_NOTIFIER_HPP_
#define ENGINE_MATH_TRANSFORM_NOTIFIER_HPP_

#include <functional>

namespace Engine
{
namespace Math
{
    enum class ETransformNotification
    {
        Changed,
        Destroyed
    };

    class TransformNotifier
    {
    public:
        using NotificationHandler = std::function<void(ETransformNotification)>;
        using NotificationHandlerID = uint64_t;

        TransformNotifier() = default;
        ~TransformNotifier() = default;

        TransformNotifier(const TransformNotifier& other)             = delete;
        TransformNotifier(TransformNotifier&& other)                  = delete;
        TransformNotifier& operator=(const TransformNotifier& other)  = delete;
        TransformNotifier& operator=(const TransformNotifier&& other) = delete;

        NotificationHandlerID AddNotificationHandler(NotificationHandler notificationHandler);
        void NotifyChildren(ETransformNotification notification);
        bool RemoveNotificationHandler(const NotificationHandlerID& notificationHandlerID);

    private:
        std::unordered_map<NotificationHandlerID, NotificationHandler> m_notificationHandlers;
        NotificationHandlerID m_availableHandlerID = 0;
    };
}
}

#endif // ENGINE_MATH_TRANSFORM_NOTIFIER_HPP_
