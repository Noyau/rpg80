#include "pch.h"
#include "Application.h"

#include "engine/Layer.h"
#include "engine/Window.h"

#include "engine/imgui/ImGuiLayer.h"

// Application
Application::Application(int argc, char** argv)
{
    s_Instance = this;
}

Application::~Application()
{
    s_Instance = nullptr;
}

int Application::Run()
{
    if (m_Window = Window::Create(WindowArgs{ 800, 600, "RPG-80" }); m_Window != nullptr)
    {
        m_ImGuiLayer = AddLayer<ImGuiLayer>();

        while (!m_Window->ShouldClose())
        {
            EachLayer(&Layer::Update);

            m_Window->Update();
        }

        EachLayer(&Layer::Shutdown);

        m_Layers.clear();

        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}

const SharedPtr<Layer>& Application::AddLayer(SharedPtr<Layer>&& layer)
{
    const auto& addedLayer = m_Layers.emplace_back(std::move(layer));

    addedLayer->Initialize();

    return addedLayer;
}

void Application::RemoveLayer(const SharedPtr<Layer>& layer)
{
    if (const auto it = std::ranges::remove(m_Layers, layer).begin(); it != m_Layers.end())
    {
        (*it)->Shutdown();

        m_Layers.erase(it);
    }
}
