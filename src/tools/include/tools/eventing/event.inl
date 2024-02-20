#pragma once
#ifndef ENGINE_TOOLS_EVENTING_EVENT_INL_
#define ENGINE_TOOLS_EVENTING_EVENT_INL_

#include "tools/eventing/event.hpp"

namespace Eventing = Engine::Tools::Eventing;

template<class... ArgTypes>
Eventing::ListenerID Eventing::Event<ArgTypes...>::AddListener(Callback callback)
{
    ListenerID listenerID = m_availableListenerID++;
    m_callbacks.emplace(listenerID, callback);
    return listenerID;
}

template<class... ArgTypes>
Eventing::ListenerID Eventing::Event<ArgTypes...>::operator+=(Callback callback)
{
    return AddListener(callback);
}

template<class... ArgTypes>
bool Eventing::Event<ArgTypes...>::RemoveListener(ListenerID listenerID)
{
    return m_callbacks.erase(listenerID) != 0;
}

template<class... ArgTypes>
bool Eventing::Event<ArgTypes...>::operator-=(ListenerID listenerID)
{
    return RemoveListener(listenerID);
}

template<class... ArgTypes>
void Eventing::Event<ArgTypes...>::RemoveAllListeners()
{
    m_callbacks.clear();
}

template<class... ArgTypes>
uint64_t Eventing::Event<ArgTypes...>::GetListenerCount()
{
    return m_callbacks.size();
}

template<class... ArgTypes>
void Eventing::Event<ArgTypes...>::Invoke(ArgTypes... args)
{
    for (auto const& [key, value] : m_callbacks)
        value(args...);
}

#endif // ENGINE_TOOLS_EVENTING_EVENT_INL_
