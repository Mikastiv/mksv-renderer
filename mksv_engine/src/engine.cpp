#include "engine.hpp"

#include "log.hpp"
#include "window.hpp"
#include "windows_helpers.hpp"
#include "wrapper/windows.h"

namespace mksv
{
constexpr u32 CLASS_ALREADY_REGISTERED = 1410;

static auto CALLBACK WndProc( HWND h_wnd, UINT message, WPARAM w_param, LPARAM l_param ) -> LRESULT;

static auto register_window_class( const HINSTANCE h_instance ) -> bool
{
    const WNDCLASSEX wc{
        .cbSize = sizeof( wc ),
        .style = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = &WndProc,
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hInstance = h_instance,
        .hIcon = LoadIcon( NULL, IDI_APPLICATION ),
        .hCursor = LoadCursor( NULL, IDC_ARROW ),
        .hbrBackground = reinterpret_cast<HBRUSH>( COLOR_WINDOW + 1 ),
        .lpszMenuName = nullptr,
        .lpszClassName = Window::CLASS_NAME,
        .hIconSm = LoadIcon( NULL, IDI_APPLICATION ) };

    const ATOM class_id = RegisterClassExW( &wc );

    if ( class_id == 0 && GetLastError() != CLASS_ALREADY_REGISTERED ) {
        mksv::log_last_window_error();
        return false;
    }

    return true;
}

auto new_engine( const HINSTANCE h_instance ) -> std::expected<Engine, EngineError>
{
    if ( !register_window_class( h_instance ) ) {
        return std::unexpected{ EngineError::RegisterClassFailed };
    }

    return Engine{ h_instance };
}

Engine::Engine( const HINSTANCE h_instance )
    : _h_instance{ h_instance }
{
}

Engine::Engine( Engine&& other )
    : _h_instance{ other._h_instance }
{
    other._h_instance = nullptr;
}

auto Engine::operator=( Engine&& other ) -> Engine&
{
    if ( this == &other ) {
        return *this;
    }

    _h_instance = other._h_instance;
    other._h_instance = nullptr;

    return *this;
}

Engine::~Engine()
{
    if ( _h_instance != nullptr ) {
        UnregisterClassW( Window::CLASS_NAME, _h_instance );
    }
}

static auto CALLBACK WndProc( HWND h_wnd, UINT message, WPARAM w_param, LPARAM l_param ) -> LRESULT
{
    return DefWindowProcW( h_wnd, message, w_param, l_param );
}
} // namespace mksv
