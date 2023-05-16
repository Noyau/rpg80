#pragma once

#include "engine/opengl/GraphicsContext.h"
#include "engine/opengl/GraphicsFramework.h"

// Window Args
struct WindowArgs
{
    uint32_t m_Width;
    uint32_t m_Height;
    std::string m_Title;
    bool m_Resizable{ true };
    bool m_VSync{ true };
};

// Window
class Window
{
public:
    Window() = delete;
    Window(const Window&) = delete;
    Window(Window&&) = delete;

    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;

    bool ShouldClose() const;

    void Update();

private:
    Window(UniquePtr<GraphicsFramework>&& framework, UniquePtr<GraphicsContext>&& context);

public:
    static UniquePtr<Window> Create(const WindowArgs& args);

private:
    UniquePtr<GraphicsFramework> m_Framework;
    UniquePtr<GraphicsContext> m_Context;
};