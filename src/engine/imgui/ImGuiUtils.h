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

    // Menu Item
    struct MenuItem final
    {
        bool m_Selected{ false };
        bool m_DisableOnSelect{ false };
        std::string m_Label{};
        std::string m_Shortcut{};

        MenuItem() = default;
        MenuItem(const char* label, bool selected, bool disableOnSelect = false);
        MenuItem(const char* label, const char* shortcut = "", bool selected = false, bool disableOnSelect = false);

        void Update();
    };

    // Menu
    class Menu final : public std::vector<SharedPtr<MenuItem>>
    {
    public:
        Menu(const std::string& path);

        SharedPtr<MenuItem> AddItem(const char* label, bool selected, bool disableOnSelect = false);
        SharedPtr<MenuItem> AddItem(const char* label, const char* shortcut = "", bool selected = false, bool disableOnSelect = false);

        SharedPtr<MenuItem> AddToggleItem(const char* label, bool selected);
        SharedPtr<MenuItem> AddToggleItem(const char* label, const char* shortcut = "", bool selected = false);

        SharedPtr<MenuItem> AddWindowItem(const char* label, bool opened);
        SharedPtr<MenuItem> AddWindowItem(const char* label, const char* shortcut = "", bool opened = false);

        void Update();

        constexpr const std::string& GetPath() const { return m_Path; }

    private:
        std::string m_Path{};
    };

    // Menu Bar
    class MenuBar final : public std::vector<SharedPtr<Menu>>
    {
    public:
        MenuBar(bool isMain = false);

        bool Exists(const std::string& path) const;
        SharedPtr<Menu> Get(const std::string& path) const;
        SharedPtr<Menu> GetOrCreate(const std::string& path);

        void Update();

    private:
        bool m_IsMain{ false };
    };
}