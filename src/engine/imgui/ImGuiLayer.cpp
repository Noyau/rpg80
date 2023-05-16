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

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

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

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    Render();

    // Rendering
    ImGui::Render();

    glClear(GL_COLOR_BUFFER_BIT);
    GL_TEST_ERR;

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::Render()
{
    // TODO - render imgui layer

    ImGui::Begin("WINDOW");
    {
        ImGui::Text("Bonjour !");
    }
    ImGui::End();
}
