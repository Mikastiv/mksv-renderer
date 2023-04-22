#include "mksv/log.hpp"

#include "mksv/mksv_win.hpp"
#include "mksv/utils/helpers.hpp"
#include "mksv/utils/string.hpp"

#include <cassert>
#include <format>

namespace mksv
{

static auto log( const LogLevel level, const std::wstring_view msg, const std::source_location location ) -> void
{
    const auto fmt{ std::format(
        L"[{}]: {} ({}:{})\n",
        log_level_str( level ),
        msg,
        string_to_wstring( location.file_name() ),
        location.line()
    ) };
    OutputDebugString( fmt.c_str() );
}

auto log_level_str( const LogLevel level ) -> std::wstring_view
{
    switch ( level ) {
        case LogLevel::Info:
            return L"Info";
        case LogLevel::Warning:
            return L"Warning";
        case LogLevel::Error:
            return L"Error";
        case LogLevel::Fatal:
            return L"Fatal";
        default:
            assert( false && "Unknown log level" );
            return L"UNKNOWN";
    }
}

auto log_info( const std::wstring_view msg, const std::source_location location ) -> void
{
    log( LogLevel::Info, msg, location );
}

auto log_warning( const std::wstring_view msg, const std::source_location location ) -> void
{
    log( LogLevel::Warning, msg, location );
}

auto log_error( const std::wstring_view msg, const std::source_location location ) -> void
{
    log( LogLevel::Error, msg, location );
}

auto log_fatal( const std::wstring_view msg, const std::source_location location ) -> void
{
    log( LogLevel::Fatal, msg, location );
}

auto log_last_window_error( const std::source_location location ) -> void
{
    const auto error = get_last_window_error_string();
    log_error( error.get(), location );
}

} // namespace mksv
