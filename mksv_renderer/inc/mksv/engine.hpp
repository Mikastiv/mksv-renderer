#pragma once

#include "mksv/graphics/command_queue.hpp"
#include "mksv/keyboard.hpp"
#include "mksv/mksv_d3d12.hpp"
#include "mksv/mksv_win.hpp"
#include "mksv/mksv_wrl.hpp"
#include "mksv/win/window.hpp"
#include "mksv/win/window_class.hpp"

#include <memory>

namespace mksv
{
class Engine
{
    friend auto CALLBACK WndProc( HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param ) -> LRESULT;

public:
    static auto create() -> std::unique_ptr<Engine>;

public:
    Engine( const Engine& ) = delete;
    Engine( Engine&& );
    auto operator=( const Engine& ) -> Engine& = delete;
    auto operator=( Engine&& ) -> Engine&;
    ~Engine();

public:
    [[nodiscard]] auto copy_data() -> bool;
    auto               update() -> void;

private:
    Engine(
        const HINSTANCE               h_instance,
        std::unique_ptr<WindowClass>  window_class,
        std::unique_ptr<Window>       window,
        ComPtr<DXGIAdapter>           adapter,
        ComPtr<D3D12Device>           device,
        std::unique_ptr<CommandQueue> command_queue
    );

private:
    auto GetKeyboard() -> Keyboard&;

private:
    static inline u32 instance_count = 0;

    HINSTANCE                     h_instance_;
    std::unique_ptr<WindowClass>  window_class_;
    std::unique_ptr<Window>       window_;
    Keyboard                      keyboard_;
    ComPtr<DXGIAdapter>           adapter_;
    ComPtr<D3D12Device>           device_;
    std::unique_ptr<CommandQueue> command_queue_;
    ComPtr<ID3D12Resource>        vertex_buffer_;
};

} // namespace mksv
