#include "mksv/window.hpp"

#include "mksv/log.hpp"
#include "mksv/utils/helpers.hpp"

#include <cassert>

namespace mksv
{

auto mksv::new_window( const WindowProps props ) -> std::optional<Window>
{
    const HWND h_wnd = CreateWindowW(
        Window::CLASS_NAME,
        props.title.data(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        static_cast<i32>( props.width ),
        static_cast<i32>( props.height ),
        nullptr,
        nullptr,
        props.h_instance,
        nullptr
    );

    if ( h_wnd == nullptr ) {
        mksv::log_last_window_error();
        return std::nullopt;
    }

    return Window{ h_wnd, props };
}

Window::Window( const HWND h_wnd, const WindowProps props )
    : _h_wnd{ h_wnd },
      _width{ props.width },
      _height{ props.height }
{
}

Window::Window( Window&& other )
    : _h_wnd{ other._h_wnd },
      _width{ other._width },
      _height{ other._height }
{
    other._h_wnd = nullptr;
}

auto Window::operator=( Window&& other ) -> Window&
{
    if ( this == &other ) {
        return *this;
    }

    _h_wnd = other._h_wnd;
    other._h_wnd = nullptr;
    _width = other._width;
    _height = other._height;

    return *this;
}

Window::~Window()
{
    if ( _h_wnd != nullptr ) {
        DestroyWindow( _h_wnd );
    }
}

auto Window::show() const -> bool
{
    return ShowWindow( _h_wnd, SW_SHOW );
}
} // namespace mksv
