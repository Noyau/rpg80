#include "pch.h"
#include "ImGuiLayer.h"

#include "engine/Application.h"

// ImGui Layer
void ImGuiLayer::Initialize()
{
    Layer::Initialize();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.f;
        style.Colors[ImGuiCol_WindowBg].w = 1.f;
    }

    Application::Get().GetWindow()->InitializeImGui();
}

void ImGuiLayer::Shutdown()
{
    Application::Get().GetWindow()->ShutdownImGui();

    ImGui::DestroyContext();

    Layer::Shutdown();
}

void ImGuiLayer::Update()
{
    Layer::Update();

    const ImGuiIO& io{ ImGui::GetIO() };

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    RenderDockSpace(io);

    // Rendering
    ImGui::Render();

    glClear(GL_COLOR_BUFFER_BIT);
    GL_TEST_ERR;

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void ImGuiLayer::RenderDockSpace(const ImGuiIO& io) const
{
    static bool fullScreen{ true };
    static bool menuBar{ false };

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;

    if (menuBar)
    {
        windowFlags |= ImGuiWindowFlags_MenuBar;
    }

    IMGUI_DOCK_SPACE(io, fullScreen, windowFlags, ImGuiDockNodeFlags_PassthruCentralNode)
    {
        Application::Get().EachLayer([&io](const SharedPtr<Layer>& layer) { layer->RenderImGui(io); });
    }
}

void ImGuiLayer::RenderImGui(const ImGuiIO& io)
{
    Layer::RenderImGui(io);

    // todo - remove this when tests are done!
    IMGUI_SCOPE(MainMenuBar)
    {
        IMGUI_SCOPE(Menu, "File")
        {
            ImGui::MenuItem("Show demo", nullptr, &m_ShowDemo);
        }
    }

    if (m_ShowDemo)
    {
        ImGui::ShowDemoWindow(&m_ShowDemo);
    }
    // todo - remove this when tests are done!
}
