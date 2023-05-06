#include "mksv/win/window.hpp"

#include "mksv/log.hpp"
#include "mksv/utils/helpers.hpp"

#include <cassert>

namespace mksv
{

auto Window::create( const WindowProps props, ComPtr<DXGIFactory> factory, ComPtr<D3D12CommandQueue> queue )
    -> std::unique_ptr<Window>
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

    u32     allow_tearing = 0;
    HRESULT hr =
        factory->CheckFeatureSupport( DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allow_tearing, sizeof( allow_tearing ) );
    if ( FAILED( hr ) ) {
        log_hresult( hr );
        allow_tearing = 0;
    }

    const DXGI_SWAP_CHAIN_DESC1 desc = {
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
    hr = factory->CreateSwapChainForHwnd( queue.Get(), h_wnd, &desc, nullptr, nullptr, &swapchain1 );
    if ( FAILED( hr ) || FAILED( swapchain1.As( &swapchain ) ) ) {
        log_hresult( hr );
        DestroyWindow( h_wnd );
        return nullptr;
    }

    return std::unique_ptr<Window>{ new Window( h_wnd, props, std::move( swapchain ) ) };
}

Window::Window( const HWND h_wnd, WindowProps props, ComPtr<DXGISwapChain> swapchain )
    : h_wnd_{ h_wnd },
      props_{ std::move( props ) },
      swapchain_{ std::move( swapchain ) }
{
}

Window::Window( Window&& other )
    : h_wnd_{ other.h_wnd_ },
      props_{ std::move( other.props_ ) },
      swapchain_{ std::move( other.swapchain_ ) }
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
    swapchain_ = std::move( other.swapchain_ );
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
