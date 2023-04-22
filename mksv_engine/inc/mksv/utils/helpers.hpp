#pragma once

#include "mksv/mksv_win.hpp"

#include <memory>

namespace mksv
{
using WindowErrorString = std::unique_ptr<wchar_t, decltype( &LocalFree )>;

auto get_last_window_error_string() -> WindowErrorString;
} // namespace mksv
