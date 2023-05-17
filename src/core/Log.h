#pragma once

#include <spdlog/spdlog.h>

// Logger
using Logger = spdlog::logger;

// Log
class Log final
{
public:
    Log() = delete;
    Log(const Log&) = delete;
    Log(Log&&) = delete;

    Log& operator=(const Log&) = delete;
    Log& operator=(Log&&) = delete;

public:
    static void Initialize();

    static const SharedPtr<Logger>& GetLogger() { return s_Logger; }

private:
    static inline SharedPtr<Logger> s_Logger{ nullptr };
};

// Macros
#define LOG_EXCEPTION(ex)       LOG_TRACE(ex.what())
#define LOG_CRITICAL(...)       LOG(critical, __VA_ARGS__)
#define LOG_TRACE(...)          LOG(trace, __VA_ARGS__)
#define LOG_ERROR(...)          LOG(error, __VA_ARGS__)
#define LOG_WARNING(...)        LOG(warn, __VA_ARGS__)
#define LOG_DEBUG(...)          LOG(debug, __VA_ARGS__)
#define LOG_INFO(...)           LOG(info, __VA_ARGS__)

#define LOG(level, ...)         \
    Log::GetLogger()->level(__VA_ARGS__)
