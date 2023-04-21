#include "mksv/window.hpp"
#include "window.hpp"

#include <cassert>

namespace mksv
{

static constexpr const wchar_t* const WINDOW_CLASS_NAME = L"MksvEngineWindowClass";

auto mksv::new_window( const WindowProps props ) -> std::expected<Window, HRESULT>
{
    const WNDCLASSEX wc{
        .cbSize = sizeof( wc ),
        .style = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = DefWindowProc,
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hInstance = props.h_instance,
        .hIcon = LoadIcon( NULL, IDI_APPLICATION ),
        .hCursor = LoadCursor( NULL, IDC_ARROW ),
        .hbrBackground = reinterpret_cast<HBRUSH>( COLOR_WINDOW + 1 ),
        .lpszMenuName = nullptr,
        .lpszClassName = WINDOW_CLASS_NAME,
        .hIconSm = LoadCursor( NULL, IDC_ARROW ) };

    [[maybe_unused]] const ATOM class_id = RegisterClassEx( &wc );
    // TODO: handle errors better
    assert( class_id != 0 );

    return std::unexpected{ E_FAIL };
}
} // namespace mksv
