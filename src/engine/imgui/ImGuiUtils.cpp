#include "pch.h"
#include "ImGuiUtils.h"

namespace ImGui::Utils
{
    // Window
    Window::Window(bool opened, void(*endCallback)())
        : m_Opened(opened)
        , m_EndCallback(endCallback)
    {}

    Window::~Window()
    {
        if (m_EndCallback != nullptr)
        {
            m_EndCallback();
        }
    }

    // ImGui Scope
    Scope::Scope(bool hasBegun, void(*endCallback)())
        : m_EndCallback(hasBegun ? endCallback : nullptr)
    {}

    Scope::~Scope()
    {
        if (m_EndCallback != nullptr)
        {
            m_EndCallback();
        }
    }

    // Tree Node
    TreeNode::TreeNode(const char* label)
        : TreeNode(label, false)
    {}

    TreeNode::TreeNode(const char* label, bool defaultOpen)
        : m_Opened(TreeNodeEx(label, TreeNodeFlag(defaultOpen)))
    {}

    TreeNode::~TreeNode()
    {
        if (m_Opened)
        {
            TreePop();
        }
    }

    // Scoped Style Var
    ScopedStyleVar::~ScopedStyleVar()
    {
        PopStyleVar();
    }

    void ScopedStyleVar::PushStyleVar(ImGuiStyleVar var, float value)
    {
        ImGui::PushStyleVar(var, value);
        ++m_Count;
    }

    void ScopedStyleVar::PushStyleVar(ImGuiStyleVar var, const ImVec2& value)
    {
        ImGui::PushStyleVar(var, value);
        ++m_Count;
    }

    void ScopedStyleVar::PopStyleVar()
    {
        if (m_Count > 0)
        {
            PopStyleVar(m_Count);
        }
    }

    void ScopedStyleVar::PopStyleVar(uint32_t count)
    {
        //SoftAssert(count >= m_Count, "Trying to pop more than it has been pushed!");

        if (count >= m_Count)
        {
            m_Count = 0;
        }
        else
        {
            m_Count -= count;
        }

        ImGui::PopStyleVar(count);
    }

    // Dock Space
    DockSpace::DockSpace(const ImGuiIO& io, bool fullScreen, ImGuiWindowFlags windowFlags, ImGuiDockNodeFlags dockSpaceFlags /*= ImGuiDockNodeFlags_None*/)
    {
        ScopedStyleVar varScope;

        if (fullScreen)
        {
            if (const ImGuiViewport* viewport = GetMainViewport())
            {
                SetNextWindowPos(viewport->WorkPos);
                SetNextWindowSize(viewport->WorkSize);
                SetNextWindowViewport(viewport->ID);
            }

            varScope.PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            varScope.PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

            windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockSpaceFlags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockSpaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
        {
            windowFlags |= ImGuiWindowFlags_NoBackground;
        }

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.

        varScope.PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.f, 0.f }); // remove padding

        Begin("DockSpace", nullptr, windowFlags);

        varScope.PopStyleVar();

        // Submit the DockSpace
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            const ImGuiID id = GetID("#DockSpace");

            ImGui::DockSpace(id, ImVec2{ 0.f, 0.f }, dockSpaceFlags);
        }
    }

    DockSpace::~DockSpace()
    {
        End();
    }

    // Menu Item
    MenuItem::MenuItem(const char* label, bool selected, bool disableOnSelect /*= false*/)
        : MenuItem(label, "", selected, disableOnSelect)
    {}

    MenuItem::MenuItem(const char* label, const char* shortcut /*= ""*/, bool selected /*= false*/, bool disableOnSelect /*= false*/)
        : m_Selected(selected)
        , m_DisableOnSelect(disableOnSelect)
        , m_Label(label)
        , m_Shortcut(shortcut)
    {}

    void MenuItem::Update()
    {
        ImGui::MenuItem(m_Label.c_str(), m_Shortcut.c_str(), &m_Selected, !m_Selected || !m_DisableOnSelect);
    }

    // Menu Item Separator
    void MenuItemSeparator::Update()
    {
        Separator();
    }

    // Menu
    Menu::Menu(const std::string& path)
        : m_Path(path)
    {}

    SharedPtr<MenuItem> Menu::AddItem(const char* label, bool selected, bool disableOnSelect /*= false*/)
    {
        return AddItem(label, "", selected, disableOnSelect);
    }

    SharedPtr<MenuItem> Menu::AddItem(const char* label, const char* shortcut /*= ""*/, bool selected /*= false*/, bool disableOnSelect /*= false*/)
    {
        return emplace_back(MakeShared<MenuItem>(label, shortcut, selected, disableOnSelect));
    }

    SharedPtr<MenuItem> Menu::AddToggleItem(const char* label, bool selected)
    {
        return AddToggleItem(label, "", selected);
    }

    SharedPtr<MenuItem> Menu::AddToggleItem(const char* label, const char* shortcut /*= ""*/, bool selected /*= false*/)
    {
        return AddItem(label, shortcut, selected, false);
    }

    SharedPtr<MenuItem> Menu::AddWindowItem(const char* label, bool opened)
    {
        return AddWindowItem(label, "", opened);
    }

    SharedPtr<MenuItem> Menu::AddWindowItem(const char* label, const char* shortcut /*= ""*/, bool opened /*= false*/)
    {
        return AddItem(label, shortcut, opened, true);
    }

    SharedPtr<MenuItemSeparator> Menu::AddSeparator()
    {
        return static_pointer_cast<MenuItemSeparator>(emplace_back(MakeShared<MenuItemSeparator>()));
    }

    void Menu::Update()
    {
        if (!empty())
        {
            IMGUI_SCOPE(Menu, m_Path.c_str())
            {
                std::ranges::for_each(*this, &MenuItem::Update);
            }
        }
    }

    // Menu Bar
    MenuBar::MenuBar(bool isMain /*=false*/)
        : m_IsMain(isMain)
    {}

    bool MenuBar::Exists(const std::string& path) const
    {
        return std::ranges::find(*this, path, &Menu::GetPath) != end();
    }

    SharedPtr<Menu> MenuBar::Get(const std::string& path) const
    {
        if (const auto it = std::ranges::find(*this, path, &Menu::GetPath); it != end())
        {
            return *it;
        }

        return nullptr;
    }

    SharedPtr<Menu> MenuBar::GetOrCreate(const std::string& path)
    {
        if (auto menu = Get(path); menu != nullptr)
        {
            return menu;
        }

        return emplace_back(MakeShared<Menu>(path));
    }

    void MenuBar::Update()
    {
        if (!empty())
        {
            if (m_IsMain)
            {
                BeginMainMenuBar();
            }
            else
            {
                BeginMenuBar();
            }

            std::ranges::for_each(*this, &Menu::Update);

            if (m_IsMain)
            {
                EndMainMenuBar();
            }
            else
            {
                EndMenuBar();
            }
        }
    }
}
