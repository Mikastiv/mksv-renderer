#pragma once

#include "mksv/mksv_win.hpp"

#include <memory>

namespace mksv
{
using WindowsErrorString = std::unique_ptr<wchar_t, decltype( &LocalFree )>;

auto windows_error_string( const DWORD error ) -> WindowsErrorString;

auto get_last_window_error_string() -> WindowsErrorString;
} // namespace mksv
