#pragma once

#include "mksv/mksv_win.hpp"

#include <memory>
#include <string>

namespace mksv
{
auto windows_error_string( const DWORD error ) -> std::wstring;

auto get_last_window_error_string() -> std::wstring;
} // namespace mksv
