#pragma once

#include "engine/Layer.h"

class Node;
class Quest;

// Game Layer
class GameLayer final : public Layer
{
public:
    void Initialize() override;
    void Shutdown() override;

    void Update() override;

    void RenderImGui(const ImGuiIO& io) override;

private:
    SharedPtr<Quest> m_Quest;
    SharedPtr<Node> m_InitialNode;
    SharedPtr<Node> m_CurrentNode;
};
