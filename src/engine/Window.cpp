#include "pch.h"
#include "Window.h"

// Window
/*static*/ UniquePtr<Window> Window::Create(const WindowArgs& args)
{
    auto framework = MakeUnique<GraphicsFramework>();

    if (!framework->IsInitialized())
    {
        LOG_CRITICAL("GLFW initialization failed!");
        return nullptr;
    }

    auto context = MakeUnique<GraphicsContext>(args);

    if (!context->IsInitialized())
    {
        LOG_CRITICAL("GLEW initialization failed!");
        return nullptr;
    }

    return UniquePtr<Window>{ new Window{ std::move(framework), std::move(context) } };
}

Window::Window(UniquePtr<GraphicsFramework>&& framework, UniquePtr<GraphicsContext>&& context)
    : m_Framework(std::move(framework))
    , m_Context(std::move(context))
{}

bool Window::ShouldClose() const
{
    return m_Context->ShouldClose();
}

void Window::Update()
{
    m_Context->SwapBuffers();
}
