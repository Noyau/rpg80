#pragma once

// Event
class Event
{
public:
    virtual ~Event() = default;

    virtual hash_t GetType() const = 0;
    virtual const char* GetName() const = 0;

    virtual std::string ToString() const { return std::string{ GetName() }; }

    friend std::ostream& operator<<(std::ostream& out, const Event& event)
    {
        return out << event.ToString();
    }
};

// Macros
#define EVENT_CLASS_TYPE(T)                                                 \
    public:                                                                 \
        constexpr hash_t GetType() const final { return StaticType; }       \
        constexpr const char* GetName() const final { return StaticName; }  \
        enum : hash_t { StaticType = Crc32(#T) };                           \
        static constexpr const char* StaticName{ #T }

// Event Type
template<typename T>
struct EventType
{
    enum : hash_t { Value = T::StaticType };
};

template<>
struct EventType<Event>
{
    enum : hash_t { Value = Crc32("Event") };
};

// Event Name
template<typename T>
struct EventName
{
    static constexpr const char* Value{ T::StaticName };
};

template<>
struct EventName<Event>
{
    static constexpr const char* Value{ "Event" };
};
