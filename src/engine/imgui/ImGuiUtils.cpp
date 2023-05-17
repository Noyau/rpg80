#include "pch.h"
#include "ImGuiUtils.h"

namespace ImGui::Utils
{
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
}