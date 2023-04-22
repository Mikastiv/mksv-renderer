#include "mksv/mksv_win.hpp"

namespace mksv
{
auto CALLBACK WndProc( HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param ) -> LRESULT
{
    switch ( msg ) {
        case WM_DESTROY: {
            PostQuitMessage( 0 );
        }
    }
    return DefWindowProcW( h_wnd, msg, w_param, l_param );
}

} // namespace mksv
