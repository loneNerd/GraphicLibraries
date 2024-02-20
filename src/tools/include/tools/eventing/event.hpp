#pragma once
#ifndef ENGINE_TOOLS_EVENTING_EVENT_HPP_
#define ENGINE_TOOLS_EVENTING_EVENT_HPP_

#include <functional>
#include <unordered_map>

namespace Engine::Tools::Eventing
{
    using ListenerID = uint64_t;

    template<class... ArgTypes>
    class Event
    {
    public:
        using Callback = std::function<void(ArgTypes...)>;

        ListenerID operator+=(Callback callback);
        bool operator-=(ListenerID listenerID);

        ListenerID AddListener(Callback callback);
        bool RemoveListener(ListenerID listenerID);
        void RemoveAllListeners();
        uint64_t GetListenerCount();
        void Invoke(ArgTypes... args);

    private:
        std::unordered_map<ListenerID, Callback> m_callbacks;
        ListenerID                               m_availableListenerID = 0;
    };
}

#include "tools/eventing/event.inl"

#endif // ENGINE_TOOLS_EVENTING_EVENT_HPP_
