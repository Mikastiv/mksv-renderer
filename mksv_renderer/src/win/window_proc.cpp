#include "mksv/engine.hpp"
#include "mksv/keycodes.hpp"
#include "mksv/log.hpp"
#include "mksv/mksv_win.hpp"

#include <cassert>
#include <format>

namespace mksv
{
union KeyFlags {
    KeyFlags( const LPARAM l_param )
        : raw_flags{ static_cast<u32>( l_param ) }
    {
    }

    struct {
        u32 repeat_count     : 16;
        u32 scan_code        : 8;
        u32 extended_key     : 1;
        u32                  : 4;
        u32 context_code     : 1;
        u32 prev_state       : 1;
        u32 transition_state : 1;
    } bits;

    u32 raw_flags;
};

static auto process_key_event( const Key key, const KeyFlags flags, Keyboard& keyboard ) -> void
{
    const KeyState state = flags.bits.transition_state ? KeyState::Up : KeyState::Down;

    Key modifier_key = Key::None;
    switch ( key ) {
        case Key::ShiftKey:
            modifier_key = flags.bits.extended_key ? Key::RShiftKey : Key::LShiftKey;
            break;
        case Key::ControlKey:
            modifier_key = flags.bits.extended_key ? Key::RControlKey : Key::LControlKey;
            break;
        case Key::AltKey:
            modifier_key = flags.bits.extended_key ? Key::RAltKey : Key::LAltKey;
            break;
    }

    if ( modifier_key != Key::None ) {
        keyboard.update_key( modifier_key, state );
    }

    keyboard.update_key( key, state );
}

auto CALLBACK WndProc( HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param ) -> LRESULT
{
    Engine* engine = reinterpret_cast<Engine*>( GetWindowLongPtrW( h_wnd, GWLP_USERDATA ) );
    // Some messages are sent on creation, so the pointer is not set yet
    if ( !engine ) {
        return DefWindowProcW( h_wnd, msg, w_param, l_param );
    }

    switch ( msg ) {
        case WM_DESTROY: {
            PostQuitMessage( 0 );
            break;
        }
        case WM_SYSKEYDOWN:
            [[fallthrough]];
        case WM_KEYDOWN:
            [[fallthrough]];
        case WM_SYSKEYUP:
            [[fallthrough]];
        case WM_KEYUP: {
            const KeyFlags flags{ l_param };
            const Key      key{ static_cast<Key>( w_param ) };
            Keyboard&      keyboard = engine->GetKeyboard();

            process_key_event( key, flags, keyboard );
            break;
        }
        default:
            return DefWindowProcW( h_wnd, msg, w_param, l_param );
    }

    return 0;
}

} // namespace mksv
