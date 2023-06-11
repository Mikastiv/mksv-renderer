#include "mksv/win/window.hpp"

#include "mksv/log.hpp"
#include "mksv/utils/helpers.hpp"

#include <cassert>

namespace mksv
{

auto Window::create(
    const WindowProps          props,
    ComPtr<DXGIFactory>        factory,
    ComPtr<ID3D12CommandQueue> queue,
    ComPtr<D3D12Device>        device
) -> std::unique_ptr<Window>
{
    RECT window_rect =
        { .left = 0, .top = 0, .right = static_cast<LONG>( props.width ), .bottom = static_cast<LONG>( props.height ) };

    AdjustWindowRect( &window_rect, WS_OVERLAPPEDWINDOW, false );

    const HWND h_wnd = CreateWindowW(
        props.class_name.c_str(),
        props.title.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        window_rect.right - window_rect.left,
        window_rect.bottom - window_rect.top,
        nullptr,
        nullptr,
        GetModuleHandleW( nullptr ),
        nullptr
    );

    if ( h_wnd == nullptr ) {
        log_last_window_error();
        return nullptr;
    }

    u32     allow_tearing = 0;
    HRESULT hr =
        factory->CheckFeatureSupport( DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allow_tearing, sizeof( allow_tearing ) );
    if ( FAILED( hr ) ) {
        log_hresult( hr );
        allow_tearing = 0;
    }

    const DXGI_SWAP_CHAIN_DESC1 swapchain_desc = {
        .Width = props.width,
        .Height = props.height,
        .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
        .Stereo = false,
        .SampleDesc = {.Count = 1, .Quality = 0},
        .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
        .BufferCount = Window::BACK_BUFFER_COUNT,
        .Scaling = DXGI_SCALING_STRETCH,
        .SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
        .AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
        .Flags = allow_tearing ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0U,
    };
    ComPtr<IDXGISwapChain1> swapchain1{};
    ComPtr<DXGISwapChain>   swapchain{};
    hr = factory->CreateSwapChainForHwnd( queue.Get(), h_wnd, &swapchain_desc, nullptr, nullptr, &swapchain1 );
    if ( FAILED( hr ) || FAILED( swapchain1.As( &swapchain ) ) ) {
        log_hresult( hr );
        DestroyWindow( h_wnd );
        return nullptr;
    }

    const D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {
        .Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
        .NumDescriptors = Window::BACK_BUFFER_COUNT,
        .Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
        .NodeMask = 0,
    };
    ComPtr<ID3D12DescriptorHeap> rtv_descriptor_heap{};
    hr = device->CreateDescriptorHeap( &heap_desc, IID_PPV_ARGS( &rtv_descriptor_heap ) );
    if ( FAILED( hr ) ) {
        log_hresult( hr );
        DestroyWindow( h_wnd );
        return nullptr;
    }

    const u32 rtv_descriptor_size = device->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_RTV );

    BackBuffers                 back_buffers;
    D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle = rtv_descriptor_heap->GetCPUDescriptorHandleForHeapStart();

    for ( u32 i = 0; i < back_buffers.size(); ++i ) {
        hr = swapchain->GetBuffer( i, IID_PPV_ARGS( &back_buffers[i] ) );
        if ( FAILED( hr ) ) {
            log_hresult( hr );
            DestroyWindow( h_wnd );
            return nullptr;
        }

        device->CreateRenderTargetView( back_buffers[i].Get(), nullptr, rtv_handle );
        rtv_handle.ptr += rtv_descriptor_size;
    }

    return std::unique_ptr<Window>{ new Window(
        h_wnd,
        props,
        std::move( swapchain ),
        std::move( rtv_descriptor_heap ),
        rtv_descriptor_size,
        std::move( back_buffers )
    ) };
}

Window::Window(
    const HWND                   h_wnd,
    WindowProps                  props,
    ComPtr<DXGISwapChain>        swapchain,
    ComPtr<ID3D12DescriptorHeap> descriptor_heap,
    const u32                    descriptor_size,
    BackBuffers                  back_buffers
)
    : h_wnd_{ h_wnd },
      props_{ std::move( props ) },
      swapchain_{ std::move( swapchain ) },
      rtv_descriptor_heap_{ std::move( descriptor_heap ) },
      rtv_descriptor_size_{ descriptor_size },
      back_buffers_{ std::move( back_buffers ) }
{
}

Window::Window( Window&& other )
    : h_wnd_{ other.h_wnd_ },
      props_{ std::move( other.props_ ) },
      swapchain_{ std::move( other.swapchain_ ) },
      rtv_descriptor_heap_{ std::move( other.rtv_descriptor_heap_ ) },
      rtv_descriptor_size_{ other.rtv_descriptor_size_ },
      back_buffers_{ std::move( other.back_buffers_ ) }
{
    other.h_wnd_ = nullptr;
    other.rtv_descriptor_size_ = 0;
}

auto Window::operator=( Window&& other ) -> Window&
{
    if ( this == &other ) {
        return *this;
    }

    h_wnd_ = other.h_wnd_;
    props_ = std::move( other.props_ );
    swapchain_ = std::move( other.swapchain_ );
    rtv_descriptor_heap_ = std::move( other.rtv_descriptor_heap_ );
    rtv_descriptor_size_ = other.rtv_descriptor_size_;
    back_buffers_ = std::move( other.back_buffers_ );
    other.h_wnd_ = nullptr;
    other.rtv_descriptor_size_ = 0;

    return *this;
}

Window::~Window()
{
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

auto Window::get_back_buffer( const u32 index ) const -> const ComPtr<ID3D12Resource>&
{
    assert( index < Window::BACK_BUFFER_COUNT );
    return back_buffers_[index];
}

auto Window::get_current_back_buffer_index() const -> u32
{
    return swapchain_->GetCurrentBackBufferIndex();
}

auto Window::get_render_target_view( const u32 index ) const -> D3D12_CPU_DESCRIPTOR_HANDLE
{
    D3D12_CPU_DESCRIPTOR_HANDLE handle = rtv_descriptor_heap_->GetCPUDescriptorHandleForHeapStart();

    handle.ptr += index * rtv_descriptor_size_;

    return handle;
}

auto Window::present( const bool v_sync ) -> HRESULT
{
    return swapchain_->Present( v_sync ? 1u : 0u, 0 );
}

auto Window::width() const -> u32
{
    return props_.width;
}

auto Window::height() const -> u32
{
    return props_.height;
}

} // namespace mksv
