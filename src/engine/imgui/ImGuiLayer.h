#pragma once

#include "engine/Layer.h"

// ImGui Layer
class ImGuiLayer final : public Layer
{
public:
    void Initialize() override;
    void Shutdown() override;

    void Update() override;

    void RenderImGui(const ImGuiIO& io) override;

private:
    void RenderDockSpace(const ImGuiIO& io) const;
};
