#pragma once

#ifdef _DEBUG

#define GL_TEST_ERR     GL::CheckError()

#else

#define GL_TEST_ERR

#endif

namespace GL
{
    // Error
    inline void CheckError(const std::source_location& loc = std::source_location::current())
    {
        if (const GLenum error = glGetError(); error != GL_NO_ERROR)
        {
            LOG_WARNING("{}:{} - OpenGL error: {}",
                loc.file_name(),
                loc.line(),
                reinterpret_cast<const char*>(glewGetErrorString(error))
            );

            DEBUG_BREAK;
        }
    }

    // Debug
#ifdef _DEBUG
    constexpr const char* DebugSourceToString(GLenum source)
    {
        switch (source)
        {
        case GL_DEBUG_SOURCE_API: return "API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
        case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
        case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
        case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
        case GL_DEBUG_SOURCE_OTHER:
        default:
            return "OTHER";
        }
    }

    constexpr const char* DebugTypeToString(GLenum type)
    {
        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR: return "ERROR";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED BEHAVIOR";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED BEHAVIOR";
        case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
        case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
        case GL_DEBUG_TYPE_MARKER: return "MARKER";
        case GL_DEBUG_TYPE_PUSH_GROUP: return "PUSH GROUP";
        case GL_DEBUG_TYPE_POP_GROUP: return "POP GROUP";
        case GL_DEBUG_TYPE_OTHER:
        default:
            return "OTHER";
        }
    }

    constexpr const char* DebugSeverityToString(GLenum severity)
    {
        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
        case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
        case GL_DEBUG_SEVERITY_LOW: return "LOW";
        case GL_DEBUG_SEVERITY_NOTIFICATION:
        default:
            return "NOTIFICATION";
        }
    }

    inline void DebugLog(GLenum type, GLenum severity, const std::string& message)
    {
        if (type == GL_DEBUG_TYPE_ERROR)
        {
            if (severity == GL_DEBUG_SEVERITY_HIGH)
            {
                LOG_CRITICAL(message);
            }
            else
            {
                LOG_ERROR(message);
            }
        }
        else
        {
            switch (severity)
            {
            case GL_DEBUG_SEVERITY_HIGH:
            case GL_DEBUG_SEVERITY_MEDIUM:
            case GL_DEBUG_SEVERITY_LOW:
                LOG_WARNING(message);
                break;
            default:
                LOG_INFO(message);
                break;
            }
        }
    }

    inline void RegisterDebugMessageCallback()
    {
        glEnable(GL_DEBUG_OUTPUT);
        CheckError();

        glDebugMessageCallback([](GLenum source, GLenum type, GLuint /*id*/, GLenum severity, GLsizei /*length*/, const GLchar* message, const void* /*userParam*/)
            {
                if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
                {
                    std::ostringstream out;
                    out << "** " << DebugSourceToString(source);
                    out << "|" << DebugTypeToString(type);
                    out << "|" << DebugSeverityToString(severity);
                    out << "**" << std::endl;
                    out << ">> " << message;

                    DebugLog(type, severity, out.str());

                    if (type == GL_DEBUG_TYPE_ERROR)
                    {
                        DEBUG_BREAK;
                    }
                }
            }, nullptr
        );
        CheckError();
    }
#else
    inline void RegisterDebugMessageCallback() {}
#endif

}
