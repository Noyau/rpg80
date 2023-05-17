#pragma once

#include <fmt/format.h>

template<typename... Args>
constexpr auto Format(fmt::format_string<Args...>&& fmt, Args&&... args)
{
    return fmt::format(std::move(fmt), std::forward<Args>(args)...);
}
