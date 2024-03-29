﻿#pragma once

// Layer
class Layer
{
public:
    virtual ~Layer() = default;

    virtual void Initialize() {}
    virtual void Shutdown() {}

    virtual void Update() {}

    virtual void RenderImGui(const ImGuiIO& io) {}
};
