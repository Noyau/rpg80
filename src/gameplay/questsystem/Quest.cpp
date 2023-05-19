#include "pch.h"
#include "Quest.h"

#include "gameplay/questsystem/Node.h"

// Quest
Node Quest::CreateNode()
{
    static uint64_t nextUUID{ 0 };

    return CreateNode(nextUUID++);
}

Node Quest::CreateNode(uint64_t uuid)
{
    auto node = *m_Nodes.emplace(Node{ *this, uuid }).first;

    return node;
}
