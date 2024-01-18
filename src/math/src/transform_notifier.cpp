#include "math/transform_notifier.hpp"

namespace Math = Engine::Math;

Math::TransformNotifier::NotificationHandlerID Math::TransformNotifier::AddNotificationHandler(NotificationHandler notificationHandler)
{
    NotificationHandlerID handlerID = m_availableHandlerID++;
    m_notificationHandlers.emplace(handlerID, notificationHandler);
    return handlerID;
}

void Math::TransformNotifier::NotifyChildren(ETransformNotification notification)
{
    if (!m_notificationHandlers.empty())
        for (auto const& [id, handler] : m_notificationHandlers)
            handler(notification);
}

bool Math::TransformNotifier::RemoveNotificationHandler(const NotificationHandlerID& notificationHandlerID)
{
    return m_notificationHandlers.erase(notificationHandlerID) != 0;
}
