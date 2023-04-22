#include "string/utils.hpp"

#include <cassert>
#include <stdlib.h>

namespace mksv
{

auto mksv::string_to_wstring( const std::string_view src ) -> std::wstring
{
    std::wstring dst{};
    dst.resize( src.length() );

    [[maybe_unused]] const errno_t err = mbstowcs_s( nullptr, &dst.front(), dst.length() + 1, src.data(), _TRUNCATE );
    assert( err == 0 );

    return dst;
}

} // namespace mksv
