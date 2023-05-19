#pragma once

#include "engine/eventsystem/Event.h"

// Event Handler Callback
using EventHandlerCallbackFn = std::function<void(const Event&)>;

// Event Handler
class EventHandler final
{
public:
    EventHandler(hash_t eventType, const void* context, EventHandlerCallbackFn callback)
        : m_EventType(eventType)
        , m_Context(context)
        , m_Callback(std::move(callback))
    {}

    constexpr bool IgnoreTypeCheck() const { return m_EventType == EventType<Event>::Value; }

    void Notify(const Event& event) const
    {
        if (event.GetType() == m_EventType || IgnoreTypeCheck())
        {
            m_Callback(event);
        }
    }

    friend bool operator<(const EventHandler& lhs, const EventHandler& rhs)
    {
        if (lhs.m_EventType < rhs.m_EventType)
            return true;

        if (lhs.m_Context != nullptr && (rhs.m_Context == nullptr || lhs.m_Context < rhs.m_Context))
            return true;

        return false;
    }

private:
    hash_t m_EventType;

    const void* m_Context;

    EventHandlerCallbackFn m_Callback;
};

// Macros
#define EVENT_HANDLER_DECLARE(eventType)                                                    \
    private:                                                                                \
        EventHandler m_##eventType##Handler;                                                \
    public:                                                                                 \
        virtual void Handle##eventType##Event(const eventType& event)

#define EVENT_HANDLED_INITIALIZE(className, eventType)                                      \
    m_##eventType##Handler(MakeEventHandler(*this, &className::*Handle##eventType##Event))

// Helpers
template<typename T, typename Callback>
constexpr EventHandler MakeEventHandler(Callback callback = {})
{
    auto func = [&callback](const Event& event)
    {
        callback(static_cast<const T&>(event));
    };

    return EventHandler{ EventType<T>::Value, nullptr, std::move(func) };
}

template<typename T>
constexpr EventHandler MakeEventHandler(void(*Callback)(const T&))
{
    auto func = [callback = Callback](const Event& event)
    {
        callback(static_cast<const T&>(event));
    };

    return EventHandler{ EventType<T>::Value, static_cast<const void*>(Callback), std::move(func) };
}

template<typename T, typename Owner>
constexpr EventHandler MakeEventHandler(Owner& owner, void(Owner::* Callback)(const T&))
{
    auto func = [&owner, callback = Callback](const Event& event)
    {
        (owner.*callback)(static_cast<const T&>(event));
    };

    return EventHandler{ EventType<T>::Value, reinterpret_cast<void*&>(Callback), std::move(func) };
}

template<typename T, typename Owner>
constexpr EventHandler MakeEventHandler(const Owner& owner, void(Owner::* Callback)(const T&) const)
{
    auto func = [&owner, callback = Callback](const Event& event)
    {
        (owner.*callback)(static_cast<const T&>(event));
    };

    return EventHandler{ EventType<T>::Value, reinterpret_cast<void*&>(Callback), std::move(func) };
}
