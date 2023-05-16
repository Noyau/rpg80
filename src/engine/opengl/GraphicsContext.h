#pragma once

struct WindowArgs;

// Graphics Context
class GraphicsContext
{
public:
    GraphicsContext(const WindowArgs& args);

    ~GraphicsContext();

    constexpr bool IsInitialized() const { return m_Initialized; }

    void InitializeImGui() const;
    void ShutdownImGui() const;

    bool ShouldClose() const;

    void SwapBuffers() const;

private:
    GLFWwindow* m_Window{ nullptr };
    bool m_Initialized{ false };
};
