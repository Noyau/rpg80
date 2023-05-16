#pragma once

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

private:
    SharedPtr<ImGuiLayer> m_ImGuiLayer;
    std::vector<SharedPtr<Layer>> m_Layers;
};

template <typename T, typename ... Args>
SharedPtr<T> Application::AddLayer(Args&&... args)
{
    return static_pointer_cast<T>(AddLayer(MakeShared<T>(std::forward<Args>(args)...)));
}
