#include "mksv/win/window_class.hpp"

#include "mksv/log.hpp"

#include <cassert>

namespace mksv
{
auto CALLBACK WndProc( HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param ) -> LRESULT;

auto new_window_class( std::wstring name ) -> std::unique_ptr<WindowClass>
{
    const WNDCLASSEX wc{
        .cbSize = sizeof( wc ),
        .style = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = &WndProc,
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hInstance = GetModuleHandleW( nullptr ),
        .hIcon = LoadIcon( NULL, IDI_APPLICATION ),
        .hCursor = LoadCursor( NULL, IDC_ARROW ),
        .hbrBackground = reinterpret_cast<HBRUSH>( COLOR_WINDOW + 1 ),
        .lpszMenuName = nullptr,
        .lpszClassName = name.c_str(),
        .hIconSm = LoadIcon( NULL, IDI_APPLICATION ) };

    const ATOM atom = RegisterClassExW( &wc );

    if ( atom == 0 ) {
        log_last_window_error();
        return nullptr;
    }

    return std::unique_ptr<WindowClass>( new WindowClass( atom, std::move( name ) ) );
}

WindowClass::WindowClass( WindowClass&& other )
    : atom_{ other.atom_ },
      name_{ std::move( other.name_ ) }
{
    other.atom_ = 0;
}

auto WindowClass::operator=( WindowClass&& other ) -> WindowClass&
{
    if ( this == &other ) {
        return *this;
    }

    atom_ = other.atom_;
    name_ = std::move( other.name_ );
    atom_ = 0;

    return *this;
}

WindowClass::~WindowClass()
{
    assert( atom_ != 0 );
    UnregisterClassW( MAKEINTATOM( atom_ ), GetModuleHandleW( nullptr ) );
}

auto WindowClass::get_atom() const -> ATOM
{
    return atom_;
}

auto WindowClass::get_name() const -> std::wstring_view
{
    return name_;
}

WindowClass::WindowClass( const ATOM atom, std::wstring name )
    : atom_{ atom },
      name_{ std::move( name ) }
{
}

} // namespace mksv
