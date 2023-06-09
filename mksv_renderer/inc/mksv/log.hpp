#pragma once

#include "mksv/common/types.hpp"

#include <format>
#include <source_location>
#include <string_view>

namespace mksv
{
enum class LogLevel {
    Info,
    Warning,
    Error
};

auto log_level_str( const LogLevel level ) -> std::wstring_view;

auto log_info( const std::wstring_view msg, const std::source_location location = std::source_location::current() )
    -> void;

auto log_warning( const std::wstring_view msg, const std::source_location location = std::source_location::current() )
    -> void;

auto log_error( const std::wstring_view msg, const std::source_location location = std::source_location::current() )
    -> void;

auto log_last_window_error( const std::source_location location = std::source_location::current() ) -> void;

auto log_hresult( const u32 hr, const std::source_location location = std::source_location::current() ) -> void;
} // namespace mksv
