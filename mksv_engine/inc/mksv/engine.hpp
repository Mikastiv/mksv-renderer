#pragma once

#include "mksv/mksv_win.hpp"
#include "mksv/win/window.hpp"
#include "mksv/win/window_class.hpp"

#include <memory>

namespace mksv
{
class Engine
{
    friend auto new_engine() -> std::unique_ptr<Engine>;

public:
    Engine( const Engine& ) = delete;
    Engine( Engine&& );
    auto operator=( const Engine& ) -> Engine& = delete;
    auto operator=( Engine&& ) -> Engine&;
    ~Engine() = default;

private:
    Engine( const HINSTANCE h_instance, std::unique_ptr<WindowClass> window_class, std::unique_ptr<Window> window );

private:
    HINSTANCE                    h_instance_;
    std::unique_ptr<WindowClass> window_class_;
    std::unique_ptr<Window>      window_;
};

auto new_engine() -> std::unique_ptr<Engine>;

} // namespace mksv
