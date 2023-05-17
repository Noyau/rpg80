#pragma once

#include "engine/Layer.h"

// ImGui Layer
class ImGuiLayer final : public Layer
{
private:
    using Menu = ImGui::Utils::Menu;
    using MenuBar = ImGui::Utils::MenuBar;
    using MenuItem = ImGui::Utils::MenuItem;

public:
    void Initialize() override;
    void Shutdown() override;

    void Update() override;

    void RenderImGui(const ImGuiIO& io) override;

    constexpr MenuBar& GetMenuBar() { return m_MenuBar; }
    constexpr const MenuBar& GetMenuBar() const { return m_MenuBar; }

private:
    void RenderDockSpace(const ImGuiIO& io) const;

private:
    MenuBar m_MenuBar{ true };
    SharedPtr<MenuItem> m_ShowDemo;     // todo - remove this when test are done!
    SharedPtr<MenuItem> m_ShowWindow;   // todo - remove this when test are done!
};
