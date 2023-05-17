#pragma once

#include "engine/Layer.h"

struct Tree;

// Game Layer
class GameLayer final : public Layer
{
public:
    void Initialize() override;
    void Shutdown() override;

    void Update() override;

    void RenderImGui(const ImGuiIO& io) override;

private:
    SharedPtr<Tree> m_Tree;
    uint32_t m_NodeId{ 0 };
};
