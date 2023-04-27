#include "mksv/win/window.hpp"

#include "mksv/log.hpp"
#include "mksv/utils/helpers.hpp"

#include <cassert>

namespace mksv
{

auto new_window( const WindowProps props ) -> std::unique_ptr<Window>
{
    const HWND h_wnd = CreateWindowW(
        props.class_name.c_str(),
        props.title.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        static_cast<i32>( props.width ),
        static_cast<i32>( props.height ),
        nullptr,
        nullptr,
        GetModuleHandleW( nullptr ),
        nullptr
    );

    if ( h_wnd == nullptr ) {
        log_last_window_error();
        return nullptr;
    }

    return std::unique_ptr<Window>{
        new Window{h_wnd, props}
    };
}

Window::Window( const HWND h_wnd, WindowProps props )
    : h_wnd_{ h_wnd },
      props_{ std::move( props ) }
{
}

Window::Window( Window&& other )
    : h_wnd_{ other.h_wnd_ },
      props_{ std::move( other.props_ ) }
{
    other.h_wnd_ = nullptr;
}

auto Window::operator=( Window&& other ) -> Window&
{
    if ( this == &other ) {
        return *this;
    }

    h_wnd_ = other.h_wnd_;
    props_ = std::move( other.props_ );
    other.h_wnd_ = nullptr;

    return *this;
}

Window::~Window()
{
    assert( h_wnd_ != nullptr );
    DestroyWindow( h_wnd_ );
}

auto Window::show() const -> bool
{
    return ShowWindow( h_wnd_, SW_SHOW );
}

auto Window::handle() const -> HWND
{
    return h_wnd_;
}
} // namespace mksv
