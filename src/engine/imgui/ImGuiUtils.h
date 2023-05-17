#pragma once

#define IMGUI_WINDOW(...)       if (auto TEMP_VAR(window_) = ImGui::Utils::Window{ ImGui::Begin(__VA_ARGS__), &ImGui::End })
#define IMGUI_SCOPE(name, ...)  if (auto TEMP_VAR(scope_) = ImGui::Utils::Scope{ ImGui::Begin##name(__VA_ARGS__), &ImGui::End##name })
#define IMGUI_TREE_NODE(...)    if (auto TEMP_VAR(treeNode_) = ImGui::Utils::TreeNode{ __VA_ARGS__ })
#define IMGUI_DOCK_SPACE(...)   if (auto TEMP_VAR(dockSpace_) = ImGui::Utils::DockSpace{ __VA_ARGS__ })

namespace ImGui::Utils
{
    // Window
    class Window final
    {
    public:
        Window(bool opened, void(*endCallback)());
        Window(const Window&) = delete;
        Window(Window&&) = delete;

        ~Window();

        constexpr operator bool() const { return m_Opened; }

    private:
        bool m_Opened{ false };
        void (*m_EndCallback)();
    };

    // ImGui Scope
    class Scope final
    {
    public:
        Scope(bool hasBegun, void(*endCallback)());
        Scope(const Scope&) = delete;
        Scope(Scope&&) = delete;

        ~Scope();

        constexpr operator bool() const { return m_EndCallback != nullptr; }

    private:
        void (*m_EndCallback)();
    };

    // Tree Node
    constexpr ImGuiTreeNodeFlags TreeNodeFlag(bool defaultOpen)
    {
        return defaultOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None;
    }

    class TreeNode final
    {
    public:
        TreeNode(const char* label);
        TreeNode(const char* label, bool defaultOpen);

        template<typename... Args>
        TreeNode(const char* format, Args&&... args);
        template<typename... Args>
        TreeNode(bool defaultOpen, const char* format, Args&&... args);

        TreeNode(const TreeNode&) = delete;
        TreeNode(TreeNode&&) = delete;

        ~TreeNode();

        constexpr operator bool() const { return m_Opened; }

    private:
        bool m_Opened{ false };
    };

    template <typename ... Args>
    TreeNode::TreeNode(const char* format, Args&&... args)
        : TreeNode(false, format, std::forward<Args>(args)...)
    {}

    template <typename ... Args>
    TreeNode::TreeNode(bool defaultOpen, const char* format, Args&&... args)
        : m_Opened(TreeNodeEx(format, TreeNodeFlag(defaultOpen), format, std::forward<Args>(args)...))
    {}

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