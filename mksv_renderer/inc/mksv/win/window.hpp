#pragma once

#include "mksv/common/types.hpp"
#include "mksv/mksv_d3d12.hpp"
#include "mksv/mksv_win.hpp"
#include "mksv/mksv_wrl.hpp"

#include <memory>
#include <string_view>

namespace mksv
{

struct WindowProps {
    u32          width;
    u32          height;
    std::wstring title;
    std::wstring class_name;
};

class Window
{
public:
    static inline constexpr u32 BACK_BUFFER_COUNT = 3;

public:
    static auto create( WindowProps props, ComPtr<DXGIFactory> factory, ComPtr<D3D12CommandQueue> queue )
        -> std::unique_ptr<Window>;

public:
    Window( const Window& ) = delete;
    Window( Window&& );
    auto operator=( const Window& ) -> Window& = delete;
    auto operator=( Window&& ) -> Window&;
    ~Window();

public:
    auto show() const -> bool;
    auto handle() const -> HWND;

private:
    Window( const HWND h_wnd, WindowProps props, ComPtr<DXGISwapChain> swapchain );

private:
    HINSTANCE             h_instance_;
    HWND                  h_wnd_;
    WindowProps           props_;
    ComPtr<DXGISwapChain> swapchain_;
};

} // namespace mksv
