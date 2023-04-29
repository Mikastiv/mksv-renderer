#include "mksv/utils/helpers.hpp"

namespace mksv
{
auto windows_error_string( const DWORD error_code ) -> WindowsErrorString
{
    wchar_t*   message = nullptr;
    const auto result = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        error_code,
        MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
        reinterpret_cast<wchar_t*>( &message ),
        0,
        nullptr
    );

    if ( result == 0 ) {
        message = nullptr;
    }

    return WindowsErrorString{ message, &LocalFree };
}

auto get_last_window_error_string() -> WindowsErrorString
{
    return windows_error_string( GetLastError() );
}
} // namespace mksv
