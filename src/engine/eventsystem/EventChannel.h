#pragma once

#include "engine/eventsystem/Event.h"
#include "engine/eventsystem/EventHandler.h"

// Event Channel
class EventChannel final
{
public:
    EventChannel() : EventChannel("EventChannel") {}
    EventChannel(const char* name) : m_Name(name) {}

    const std::string& GetName() const { return m_Name; }

    bool IsConnected(const EventHandler& handler) const { return m_Handlers.contains(handler); }

    void Connect(const EventHandler& handler) { m_Handlers.insert(handler); }
    void Disconnect(const EventHandler& handler) { m_Handlers.erase(handler); }

    void DisconnectAll() { m_Handlers.clear(); }

    void Notify(const Event& event) const
    {
        for (auto&& handler : m_Handlers)
        {
            handler.Notify(event);
        }
    }

    uint32_t GetCount() const { return static_cast<uint32_t>(m_Handlers.size()); }

    bool IsEmpty() const { return m_Handlers.empty(); }

private:
    std::string m_Name;
    std::set<EventHandler> m_Handlers{};
};
