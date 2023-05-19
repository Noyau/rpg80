#include "pch.h"
#include "Scene.h"

#include "gameplay/scene/Entity.h"

// Scene
Entity Scene::CreateEntity()
{
    static uint64_t nexId{ 0 };

    return CreateEntity(nexId++);
}

Entity Scene::CreateEntity(uint64_t id)
{
    auto entity = *m_Entities.emplace(Entity{ *this }).first;

    entity.AddComponent<IdComponent>(id);

    return entity;
}

void Scene::DestroyEntity(Entity entity)
{
    m_Registry.destroy(entity);
}
