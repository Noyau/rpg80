#include "pch.h"
#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

// Log
/*static*/ void Log::Initialize()
{
    using SinkPtr = spdlog::sink_ptr;
    using ColorSink = spdlog::sinks::stdout_color_sink_mt;
    using FileSink = spdlog::sinks::basic_file_sink_mt;

    std::array<SinkPtr, 2> logSinks
    {
        {
            MakeShared<ColorSink>(),
            MakeShared<FileSink>("Logs/engine.log")
        }
    };

    const auto& [colorSink, fileSink] = logSinks;

    colorSink->set_pattern("%^[%T] %n: %v%$");
    fileSink->set_pattern("[%T] [%l] %n: %v");

    auto createLogger = [&logSinks](const std::string& name)
    {
        auto logger{ MakeShared<Logger>(name, logSinks.begin(), logSinks.end()) };
        spdlog::register_logger(logger);
        logger->set_level(spdlog::level::trace);
        logger->flush_on(spdlog::level::trace);
        return logger;
    };

    s_Logger = createLogger("ENGINE");
}
