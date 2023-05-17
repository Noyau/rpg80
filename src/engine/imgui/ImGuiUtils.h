#pragma once

#define IMGUI_DOCK_SPACE(...)    \
    if (auto dockSpace = ImGui::Utils::DockSpace{ __VA_ARGS__ })

namespace ImGui::Utils
{
    // Scoped Style Var
    class ScopedStyleVar
    {
    public:
        ScopedStyleVar() = default;
        ~ScopedStyleVar();

        void PushStyleVar(ImGuiStyleVar var, float value);
        void PushStyleVar(ImGuiStyleVar var, const ImVec2& value);

        void PopStyleVar();
        void PopStyleVar(uint32_t count);

        constexpr operator bool() const { return true; }

    private:
        uint32_t m_Count{ 0 };
    };

    // Dock Space
    class DockSpace
    {
    public:
        DockSpace(const ImGuiIO& io, bool fullScreen, ImGuiWindowFlags windowFlags, ImGuiDockNodeFlags dockSpaceFlags = ImGuiDockNodeFlags_None);
        ~DockSpace();

        constexpr operator bool() const { return true; }
    };
}