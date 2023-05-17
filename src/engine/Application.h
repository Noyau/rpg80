#pragma once

#include "engine/Window.h"

class ImGuiLayer;
class Layer;

// Application
class Application
{
public:
    Application(int argc, char** argv);

    ~Application();

    int Run();

    const SharedPtr<Layer>& AddLayer(SharedPtr<Layer>&& layer);
    void RemoveLayer(const SharedPtr<Layer>& layer);

    template<typename T, typename... Args>
    SharedPtr<T> AddLayer(Args&&... args);

    template<typename Func>
    void EachLayer(Func func = {}) const;

    Window* GetWindow() const { return m_Window.get(); }

public:
    static Application& Get() { return *s_Instance; }

private:
    static inline Application* s_Instance{ nullptr };

private:
    UniquePtr<Window> m_Window;
    SharedPtr<ImGuiLayer> m_ImGuiLayer;
    std::vector<SharedPtr<Layer>> m_Layers;
};

template <typename T, typename ... Args>
SharedPtr<T> Application::AddLayer(Args&&... args)
{
    return static_pointer_cast<T>(AddLayer(MakeShared<T>(std::forward<Args>(args)...)));
}

template <typename Func>
void Application::EachLayer(Func func /*= {}*/) const
{
    std::ranges::for_each(m_Layers, std::move(func));
}
