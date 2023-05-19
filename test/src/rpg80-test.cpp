#include "pch.h"

#include "engine/eventsystem/Event.h"
#include "engine/eventsystem/EventChannel.h"
#include "engine/eventsystem/EventHandler.h"
#include "engine/eventsystem/EventListener.h"
#include "engine/eventsystem/EventSystem.h"

namespace rpg80test
{
    class E : public Event
    {
        EVENT_CLASS_TYPE(E);
    };

    TEST_CLASS(TestEventSystem)
    {
    public:
        TEST_METHOD(TestEventChannel)
        {
            const auto eh1 = MakeEventHandler<E>([](const E& e) { LOG("eh1: {}", e.ToString()); });
            const auto eh2 = MakeEventHandler(*this, &TestEventSystem::OwnedCallback);
            const auto eh3 = MakeEventHandler(*this, &TestEventSystem::ConstCallback);
            const auto eh4 = MakeEventHandler(&TestEventSystem::StaticCallback);

            EventChannel ech;

            LOG("Test unique insertions --");
            ech.Connect(eh1);    Assert::IsTrue(ech.IsConnected(eh1) && ech.GetCount() == 1, L"Failed to insert a handler with lambda.");
            ech.Disconnect(eh1); Assert::IsTrue(!ech.IsConnected(eh1) && ech.IsEmpty(), L"Failed to disconnect a handler with lambda.");

            ech.Connect(eh2);    Assert::IsTrue(ech.IsConnected(eh2) && ech.GetCount() == 1, L"Failed to insert a handler with owned callback.");
            ech.Disconnect(eh2); Assert::IsTrue(!ech.IsConnected(eh2) && ech.IsEmpty(), L"Failed to disconnect a handler with owned callback.");

            ech.Connect(eh3);    Assert::IsTrue(ech.IsConnected(eh3) && ech.GetCount() == 1, L"Failed to insert a handler with owned const callback.");
            ech.Disconnect(eh3); Assert::IsTrue(!ech.IsConnected(eh3) && ech.IsEmpty(), L"Failed to disconnect a handler with owned const callback.");

            ech.Connect(eh4);    Assert::IsTrue(ech.IsConnected(eh4) && ech.GetCount() == 1, L"Failed to insert a handler with static callback.");
            ech.Disconnect(eh4); Assert::IsTrue(!ech.IsConnected(eh4) && ech.IsEmpty(), L"Failed to disconnect a handler with static callback.");
            LOG("All OK!");

            LOG("Test multiple insertions --");
            ech.Connect(eh1); Assert::IsTrue(ech.GetCount() == 1, L"Failed to insert a handler with lambda.");
            ech.Connect(eh2); Assert::IsTrue(ech.GetCount() == 2, L"Failed to insert a handler with owned callback.");
            ech.Connect(eh3); Assert::IsTrue(ech.GetCount() == 3, L"Failed to insert a handler with owned const callback.");
            ech.Connect(eh4); Assert::IsTrue(ech.GetCount() == 4, L"Failed to insert a handler with static callback.");
            LOG("All OK!");

            LOG("Test attempt to insert similar/duplicated handlers --");
            const uint32_t count = ech.GetCount();
            ech.Connect(MakeEventHandler<E>([](const E& e) {})); Assert::IsTrue(ech.GetCount() == count, L"Duplicated/similar element with lambda has been inserted by error.");
            ech.Connect(MakeEventHandler(*this, &TestEventSystem::OwnedCallback)); Assert::IsTrue(ech.GetCount() == count, L"Duplicated/similar element with owned callback has been inserted by error.");
            ech.Connect(MakeEventHandler(*this, &TestEventSystem::ConstCallback)); Assert::IsTrue(ech.GetCount() == count, L"Duplicated/similar element with owned const callback has been inserted by error.");
            ech.Connect(MakeEventHandler(&TestEventSystem::StaticCallback)); Assert::IsTrue(ech.GetCount() == count, L"Duplicated/similar element with static callback has been inserted by error.");
            ech.DisconnectAll(); Assert::IsTrue(ech.IsEmpty(), L"Failed to disconnect all handlers.");
            LOG("All OK!");

            LOG("Test callbacks --");

            m_HandledCount = s_HandledCount = 0;

            const auto handledCount = std::make_shared<uint32_t>(0);

            const auto f = [hc = handledCount](const E& e)
            {
                LOG("eh5: {}", e.ToString());
                ++(*hc);
            };

            const auto eh5 = MakeEventHandler<E>(f);

            const E e;
            ech.Connect(eh2);
            ech.Connect(eh4);
            ech.Connect(eh5);
            ech.Notify(e);

            Assert::IsTrue(*handledCount == 1, L"Failed to handle event with lambda.");
            Assert::IsTrue(m_HandledCount == 1, L"Failed to handle event with owned callback.");
            Assert::IsTrue(s_HandledCount == 1, L"Failed to handle event with static callback.");

            LOG("All OK!");
        }

        TEST_METHOD(TestEventListener)
        {
            auto handledCount = std::make_shared<uint32_t>(0);

            auto f = [handledCount](const E& e)
            {
                ++(*handledCount);
                LOG("f: {} => {}", e.ToString(), *handledCount);
            };

            const auto eh = MakeEventHandler<E>(f);

            const E e;

            EventChannel ec;

            EventListener el;
            Assert::IsFalse(el.IsConnectedTo(ec), L"Listener is connected to channel but shouldn't.");
            Assert::IsFalse(el.IsConnected(eh), L"Handler is connected to listener but shouldn't.");
            el.Connect(eh);         Assert::IsTrue(el.IsConnected(eh), L"Handler should be connected to listener but is not.");
            ec.Notify(e);           Assert::IsTrue(*handledCount == 0, L"Callback has been handled but shouldn't.");
            el.JoinChannel(ec);  Assert::IsTrue(el.IsConnectedTo(ec), L"Listener should be connected to channel but is not.");
            ec.Notify(e);           Assert::IsTrue(*handledCount == 1, L"Callback should have been handled but is not.");
            el.LeaveChannel(ec); Assert::IsFalse(el.IsConnectedTo(ec), L"Listener should be disconnected from channel but is not.");
            ec.Notify(e);           Assert::IsTrue(*handledCount == 1, L"Callback has been handled but shouldn't.");
            el.Notify(e);           Assert::IsTrue(*handledCount == 2, L"Callback should have been handled but is not.");
            el.Disconnect(eh);      Assert::IsFalse(el.IsConnected(eh), L"Handler should be disconnected from listener but is not.");
            el.Notify(e);           Assert::IsTrue(*handledCount == 2, L"Callback has been handled but shouldn't.");

            LOG("All OK!");
        }

        TEST_METHOD(TestEventDispatch)
        {
            auto handledCount = std::make_shared<uint32_t>(0);

            auto f = [handledCount](const E& e)
            {
                ++(*handledCount);
                LOG("f: {} => {}", e.ToString(), *handledCount);
            };

            const auto eh = MakeEventHandler<E>(f);

            EventChannel ec;

            EventListener el;
            el.Connect(eh);
            el.JoinChannel(ec);

            const E e;

            EventSystem::SendTo(eh, e);    Assert::IsTrue(*handledCount == 1, L"Failed to handle callback from direct call to handler.");
            EventSystem::SendTo(el, e);    Assert::IsTrue(*handledCount == 2, L"Failed to handle callback from direct call to listener.");
            EventSystem::Broadcast(ec, e); Assert::IsTrue(*handledCount == 3, L"Failed to handle callback through broadcast/channel.");

            LOG("All OK!");
        }

    private:
        void OwnedCallback(const E& e)
        {
            LOG("eh2: {}", e.ToString());
            ++m_HandledCount;
        }

        void ConstCallback(const E& e) const
        {
            LOG("eh3: {}", e.ToString());
        }

    private:
        static void StaticCallback(const E& e)
        {
            LOG("eh4: {}", e.ToString());
            ++s_HandledCount;
        }

    private:
        static inline uint32_t s_HandledCount{ 0 };

    private:
        uint32_t m_HandledCount{ 0 };
    };
}
