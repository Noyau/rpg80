#pragma once

#include "engine/eventsystem/EventHandler.h"
#include "engine/eventsystem/EventChannel.h"

// Event Listener
class EventListener final
{
public:
    EventListener() : EventListener("EventListener") {}
    EventListener(const char* name)
        : m_Name(name)
        , m_NotifyHandler(MakeEventHandler(*this, &EventListener::Notify))
    {}

    void Notify(const Event& event) const
    {
        m_NotifyChannel.Notify(event);
    }

    // Handler
    bool IsConnected(const EventHandler& handler) const { return m_NotifyChannel.IsConnected(handler); }
    void Connect(const EventHandler& handler) { m_NotifyChannel.Connect(handler); }
    void Disconnect(const EventHandler& handler) { m_NotifyChannel.Disconnect(handler); }

    // Channel
    bool IsConnectedTo(const EventChannel& channel) const { return channel.IsConnected(m_NotifyHandler); }
    void JoinChannel(EventChannel& channel) const { channel.Connect(m_NotifyHandler); }
    void LeaveChannel(EventChannel& channel) const { channel.Disconnect(m_NotifyHandler); }

private:
    std::string m_Name;
    EventHandler m_NotifyHandler;
    EventChannel m_NotifyChannel{ "NotifyChannel" };
};
