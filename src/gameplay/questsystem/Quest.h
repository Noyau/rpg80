#pragma once

#include <entt/entity/registry.hpp>

// Quest
class Quest
{
private:
    friend class Node;

public:
    Node CreateNode();
    Node CreateNode(uint64_t uuid);

private:
    entt::registry m_Registry;
    std::set<Node> m_Nodes;
};