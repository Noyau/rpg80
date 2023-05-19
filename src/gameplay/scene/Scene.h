#pragma once

#include <entt/entity/registry.hpp>

// Scene
class Scene
{
private:
    friend class Entity;

public:
    Scene() = default;

    Entity CreateEntity();
    Entity CreateEntity(uint64_t id);
    void DestroyEntity(Entity entity);

    template<typename Func>
    void EachEntity(Func func = {}) const;

private:
    entt::registry m_Registry;
    std::set<Entity> m_Entities;
};

template <typename Func>
void Scene::EachEntity(Func func) const
{
    std::ranges::for_each(m_Entities, std::move(func));
}
