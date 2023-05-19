#pragma once

#include "engine/eventsystem/Event.h"
#include "engine/eventsystem/EventHandler.h"
#include "engine/eventsystem/EventChannel.h"
#include "engine/eventsystem/EventListener.h"

// Event System
class EventSystem final
{
public:
    static void SendTo(const EventHandler& handler, const Event& event) { handler.Notify(event); }
    static void SendTo(const EventListener& listener, const Event& event) { listener.Notify(event); }
    static void Broadcast(const EventChannel& channel, const Event& event) { channel.Notify(event); }
};
