#include "pch.h"
#include "GraphicsContext.h"

#include "engine/Window.h"

// Graphics Context
GraphicsContext::GraphicsContext(const WindowArgs& args)
    : m_Window(glfwCreateWindow(args.m_Width, args.m_Height, args.m_Title.c_str(), nullptr, nullptr))
{
    // Attributes
    glfwSetWindowAttrib(m_Window, GLFW_RESIZABLE, args.m_Resizable ? GLFW_TRUE : GLFW_FALSE);

    // Callbacks
    glfwSetWindowUserPointer(m_Window, this);

    // TODO - window callbacks

    // Set current context
    glfwMakeContextCurrent(m_Window);

    glfwSwapInterval(args.m_VSync ? 1 : 0);

    m_Initialized = glewInit() == GLEW_OK;

    if (m_Initialized)
    {
        GL::RegisterDebugMessageCallback();

        LOG_INFO("================================================");
        LOG_INFO("GLFW version: {}", glfwGetVersionString());
        LOG_INFO("GLEW version: {}", reinterpret_cast<const char*>(glewGetString(GLEW_VERSION)));
        LOG_INFO("OpenGL info:");
        LOG_INFO("   Vendor:    {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
        LOG_INFO("   Renderer:  {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
        LOG_INFO("   Version:   {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
        LOG_INFO("   Shading language version: {}", reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
        LOG_INFO("================================================");

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        GL_TEST_ERR;

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        GL_TEST_ERR;
    }
}

GraphicsContext::~GraphicsContext()
{
    glfwMakeContextCurrent(nullptr);
    glfwSetWindowUserPointer(m_Window, nullptr);
    glfwDestroyWindow(m_Window);
}

bool GraphicsContext::ShouldClose() const
{
    return glfwWindowShouldClose(m_Window);
}

void GraphicsContext::SwapBuffers() const
{
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}
