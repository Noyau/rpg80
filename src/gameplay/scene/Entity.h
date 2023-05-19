#pragma once

#include <entt/entt.hpp>

#include "gameplay/components/IdComponent.h"
#include "gameplay/scene/Scene.h"

// Entity
class Entity
{
public:
    Entity() = default;
    Entity(Scene& scene)
        : m_Scene(&scene)
        , m_Handle(scene.m_Registry.create())
    {}
    Entity(const Entity&) = default;
    Entity(Entity&&) = default;

    Entity& operator=(const Entity&) = default;
    Entity& operator=(Entity&&) = default;

    template<typename T, typename... Args>
    T& AddComponent(Args&&... args);

    template<typename T>
    T& GetComponent() const;

    template<typename T>
    bool HasComponent() const;

    template<typename T>
    void RemoveComponent();

    constexpr bool IsValid() const { return m_Handle != entt::null; }

    constexpr operator bool() const { return IsValid(); }
    constexpr operator entt::entity() const { return m_Handle; }
    constexpr operator uint32_t() const { return static_cast<uint32_t>(m_Handle); }

    constexpr bool Equals(const Entity& other) const { return m_Handle == other.m_Handle && m_Scene == other.m_Scene; }

    constexpr bool operator==(const Entity& other) const { return Equals(other); }
    constexpr bool operator!=(const Entity& other) const { return !Equals(other); }

    constexpr bool operator<(const Entity& other) const { return m_Handle < other.m_Handle; }

    uint64_t GetId() const { return GetComponent<IdComponent>().m_Id; }

private:
    Scene* m_Scene{ nullptr };
    entt::entity m_Handle{ entt::null };
};

template <typename T, typename ... Args>
T& Entity::AddComponent(Args&&... args)
{
    return m_Scene->m_Registry.emplace<T>(m_Handle, std::forward<Args>(args)...);
}

template <typename T>
T& Entity::GetComponent() const
{
    return m_Scene->m_Registry.get<T>(m_Handle);
}

template <typename T>
bool Entity::HasComponent() const
{
    return m_Scene->m_Registry.any_of<T>(m_Handle);
}

template <typename T>
void Entity::RemoveComponent()
{
    m_Scene->m_Registry.remove<T>();
}
