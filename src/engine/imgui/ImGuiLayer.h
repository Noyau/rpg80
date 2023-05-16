#pragma once

#include "engine/Layer.h"

// ImGui Layer
class ImGuiLayer final : public Layer
{
public:
    void Initialize() override;
    void Shutdown() override;

    void Update() override;
};
