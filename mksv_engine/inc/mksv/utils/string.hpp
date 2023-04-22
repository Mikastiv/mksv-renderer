#pragma once

#include <string_view>

namespace mksv
{
auto string_to_wstring( const std::string_view src ) -> std::wstring;
}
