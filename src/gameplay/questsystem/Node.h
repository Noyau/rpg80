#pragma once

#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>

#include "gameplay/questsystem/Quest.h"

// Node
class Node
{
public:
    Node() = default;
    Node(const Node&) = default;
    Node(Node&&) = default;

    Node(Quest& quest, uint64_t uuid)
        : m_Quest(&quest)
        , m_UUID(uuid)
        , m_Handle(quest.m_Registry.create())
    {}

    Node& operator=(const Node&) = default;
    Node& operator=(Node&&) = default;

    constexpr operator entt::entity() const { return m_Handle; }
    constexpr operator uint32_t() const { return static_cast<uint32_t>(m_Handle); }

    constexpr uint64_t GetUUID() const { return m_UUID; }

    template<typename T, typename... Args>
    T& AddOutcome(Args&&... args);

    template<typename T>
    T& GetOutcome() const;

    template<typename T>
    bool HasOutcome() const;

    template<typename T, typename... Args>
    T& AddComponent(Args&&... args);

    template<typename T>
    T& GetComponent() const;

    template<typename T>
    bool HasComponent() const;

    template<typename T>
    void RemoveComponent();

    constexpr bool operator<(const Node& other) const { return m_Handle < other.m_Handle; }

private:
    Quest* m_Quest{ nullptr };
    uint64_t m_UUID{ 0 };
    entt::entity m_Handle{ entt::null };
};

template <typename T, typename ... Args>
T& Node::AddOutcome(Args&&... args)
{
    return m_Quest->m_Registry.emplace<T>(m_Handle, std::forward<Args>(args)...);
}

template <typename T>
T& Node::GetOutcome() const
{
    return m_Quest->m_Registry.get<T>(m_Handle);
}

template <typename T>
bool Node::HasOutcome() const
{
    return m_Quest->m_Registry.any_of<T>(m_Handle);
}

template <typename T, typename ... Args>
T& Node::AddComponent(Args&&... args)
{
    return m_Quest->m_Registry.emplace<T>(m_Handle, std::forward<Args>(args)...);
}

template <typename T>
T& Node::GetComponent() const
{
    return m_Quest->m_Registry.get<T>(m_Handle);
}

template <typename T>
bool Node::HasComponent() const
{
    return m_Quest->m_Registry.any_of<T>(m_Handle);
}

template <typename T>
void Node::RemoveComponent()
{
    m_Quest->m_Registry.remove<T>(m_Handle);
}
