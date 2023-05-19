#pragma once

#include "engine/Layer.h"

#ifndef OLD_QUEST
class Node;
class Quest;
#else
struct Tree;
#endif

// Game Layer
class GameLayer final : public Layer
{
public:
    void Initialize() override;
    void Shutdown() override;

    void Update() override;

    void RenderImGui(const ImGuiIO& io) override;

private:
#ifndef OLD_QUEST
    SharedPtr<Quest> m_Quest;
    SharedPtr<Node> m_InitialNode;
    SharedPtr<Node> m_CurrentNode;
#else
    SharedPtr<Tree> m_Tree;
    uint32_t m_NodeId{ 0 };
#endif
};
