#include "pch.h"
#include "GraphicsFramework.h"

// Graphics Framework
GraphicsFramework::GraphicsFramework()
{
    glfwSetErrorCallback([](int errorCode, const char* description)
        {
            LOG_ERROR("GLFW Error {}: {}", errorCode, description);
            DEBUG_BREAK;
        }
    );

    m_Initialized = glfwInit() == GLFW_TRUE;
}

GraphicsFramework::~GraphicsFramework()
{
    glfwTerminate();

    glfwSetErrorCallback(nullptr);
}
