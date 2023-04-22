#pragma once

#include "wrapper/windows.h"

#include <memory>

namespace mksv
{
using WindowErrorString = std::unique_ptr<wchar_t, decltype( &LocalFree )>;

auto get_last_window_error_string() -> WindowErrorString;
} // namespace mksv
