#include "windows_helpers.hpp"

namespace mksv
{
auto mksv::get_last_window_error_string() -> WindowErrorString
{
    const auto error_code = GetLastError();

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

    return WindowErrorString{ message, &LocalFree };
}
} // namespace mksv
