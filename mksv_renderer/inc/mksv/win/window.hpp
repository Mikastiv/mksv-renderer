#pragma once

#include "mksv/common/types.hpp"
#include "mksv/mksv_d3d12.hpp"
#include "mksv/mksv_win.hpp"
#include "mksv/mksv_wrl.hpp"

#include <array>
#include <memory>
#include <string>

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

    using BackBuffers = std::array<ComPtr<ID3D12Resource>, Window::BACK_BUFFER_COUNT>;

public:
    static auto create(
        WindowProps                props,
        ComPtr<DXGIFactory>        factory,
        ComPtr<ID3D12CommandQueue> queue,
        ComPtr<D3D12Device>        device
    ) -> std::unique_ptr<Window>;

public:
    Window( const Window& ) = delete;
    Window( Window&& );
    auto operator=( const Window& ) -> Window& = delete;
    auto operator=( Window&& ) -> Window&;
    ~Window();

public:
    auto show() const -> bool;
    auto handle() const -> HWND;
    auto get_back_buffer( const u32 index ) const -> const ComPtr<ID3D12Resource>&;
    auto get_current_back_buffer_index() const -> u32;
    auto get_render_target_view( const u32 index ) const -> D3D12_CPU_DESCRIPTOR_HANDLE;
    auto present( const bool v_sync ) -> HRESULT;
    auto width() const -> u32;
    auto height() const -> u32;

private:
    Window(
        const HWND                   h_wnd,
        WindowProps                  props,
        ComPtr<DXGISwapChain>        swapchain,
        ComPtr<ID3D12DescriptorHeap> descriptor_heap,
        const u32                    descriptor_size,
        BackBuffers                  back_buffers
    );

private:
    HINSTANCE             h_instance_;
    HWND                  h_wnd_;
    WindowProps           props_;
    ComPtr<DXGISwapChain> swapchain_;

    ComPtr<ID3D12DescriptorHeap> rtv_descriptor_heap_;
    u32                          rtv_descriptor_size_;
    BackBuffers                  back_buffers_;
};

} // namespace mksv
