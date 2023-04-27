#pragma once

#include "mksv/keyboard.hpp"
#include "mksv/mksv_win.hpp"
#include "mksv/win/window.hpp"
#include "mksv/win/window_class.hpp"

#include <memory>

namespace mksv
{
class Engine
{
    friend auto CALLBACK WndProc( HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param ) -> LRESULT;
    friend auto          new_engine() -> std::unique_ptr<Engine>;

public:
    Engine( const Engine& ) = delete;
    Engine( Engine&& );
    auto operator=( const Engine& ) -> Engine& = delete;
    auto operator=( Engine&& ) -> Engine&;
    ~Engine();

private:
    Engine( const HINSTANCE h_instance, std::unique_ptr<WindowClass> window_class, std::unique_ptr<Window> window );

private:
    auto GetKeyboard() -> Keyboard&;

private:
    static inline u32 instance_count = 0;

    HINSTANCE                    h_instance_;
    std::unique_ptr<WindowClass> window_class_;
    std::unique_ptr<Window>      window_;
    Keyboard                     keyboard_;
};

auto new_engine() -> std::unique_ptr<Engine>;

} // namespace mksv
