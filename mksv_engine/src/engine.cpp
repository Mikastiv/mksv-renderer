#include "mksv/engine.hpp"

#include "mksv/log.hpp"
#include "mksv/mksv_win.hpp"
#include "mksv/utils/helpers.hpp"

#include <cassert>

namespace mksv
{
auto new_engine() -> std::unique_ptr<Engine>
{
    const HINSTANCE h_instance = GetModuleHandleW( nullptr );
    assert( h_instance );

    auto window_class = new_window_class();
    if ( !window_class ) {
        return nullptr;
    }

    auto window = new_window(
        WindowProps{ .width = 800, .height = 600, .title = L"MKSV Engine", .class_name = window_class->get_name().data() }
    );

    if ( !window ) {
        return nullptr;
    }

    window->show();

    return std::unique_ptr<Engine>{
        new Engine{h_instance, std::move( window_class ), std::move( window )}
    };
}

Engine::Engine( Engine&& other )
    : h_instance_{ other.h_instance_ },
      window_class_{ std::move( other.window_class_ ) }
{
    other.h_instance_ = nullptr;
}

auto Engine::operator=( Engine&& other ) -> Engine&
{
    if ( this == &other ) {
        return *this;
    }

    h_instance_ = other.h_instance_;
    window_class_ = std::move( other.window_class_ );
    window_ = std::move( other.window_ );
    other.h_instance_ = nullptr;

    return *this;
}

Engine::~Engine()
{
    --instance_count;
}

Engine::Engine( const HINSTANCE h_instance, std::unique_ptr<WindowClass> window_class, std::unique_ptr<Window> window )
    : h_instance_{ h_instance },
      window_class_{ std::move( window_class ) },
      window_{ std::move( window ) }
{
    assert( instance_count == 0 && "Only 1 engine instance can exist at a time" );
    ++instance_count;
}
} // namespace mksv
