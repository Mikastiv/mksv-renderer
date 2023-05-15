#include "mksv/utils/helpers.hpp"

namespace mksv
{
auto windows_error_string( const DWORD error_code ) -> std::wstring
{
    wchar_t*   message = nullptr;
    const auto result = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        error_code,
        0,
        reinterpret_cast<wchar_t*>( &message ),
        0,
        nullptr
    );

    if ( result == 0 ) {
        return std::wstring{};
    }

    std::wstring error{ message };
    LocalFree( message );
    const size_t index = error.rfind( L"\r\n" );
    if ( index != std::wstring::npos ) {
        error.erase( index );
    }

    return error;
}

auto get_last_window_error_string() -> std::wstring
{
    return windows_error_string( GetLastError() );
}
} // namespace mksv
