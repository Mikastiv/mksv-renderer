#include "mksv/engine.hpp"

#include "mksv/common/types.hpp"
#include "mksv/log.hpp"
#include "mksv/math/consts.hpp"
#include "mksv/math/types.hpp"
#include "mksv/utils/d3d12_helpers.hpp"
#include "mksv/utils/helpers.hpp"

#include <cassert>
#include <chrono>
#include <cmath>
#include <ranges>

namespace mksv
{
struct Vertex {
    vec3 pos;
    vec3 col;
};

auto Engine::create() -> std::unique_ptr<Engine>
{
    const HINSTANCE h_instance = GetModuleHandleW( nullptr );
    assert( h_instance );

    SetThreadDpiAwarenessContext( DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 );

    HRESULT hr = E_FAIL;
    u32     create_factory_flags = 0;

#ifdef _DEBUG
    ComPtr<D3D12Debug> debug_interface{};
    hr = D3D12GetDebugInterface( IID_PPV_ARGS( &debug_interface ) );
    if ( FAILED( hr ) ) {
        log_hresult( hr );
    } else {
        debug_interface->EnableDebugLayer();
        debug_interface->SetEnableGPUBasedValidation( true );
        create_factory_flags = DXGI_CREATE_FACTORY_DEBUG;
    }
#endif

    ComPtr<DXGIFactory> dxgi_factory{};
    hr = CreateDXGIFactory2( create_factory_flags, IID_PPV_ARGS( &dxgi_factory ) );
    if ( FAILED( hr ) ) {
        log_hresult( hr );
        return nullptr;
    }

    ComPtr<DXGIAdapter> dxgi_adapter{};
    hr = dxgi_factory->EnumAdapterByGpuPreference( 0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS( &dxgi_adapter ) );
    if ( FAILED( hr ) ) {
        log_hresult( hr );
        return nullptr;
    }

    ComPtr<D3D12Device> d3d12_device{};
    hr = D3D12CreateDevice( dxgi_adapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS( &d3d12_device ) );
    if ( FAILED( hr ) ) {
        log_hresult( hr );
        return nullptr;
    }

    D3D12_FEATURE_DATA_FEATURE_LEVELS levels{};
    D3D_FEATURE_LEVEL                 levels_array[] = {
        D3D_FEATURE_LEVEL_12_2,
    };
    levels.NumFeatureLevels = static_cast<u32>( std::size( levels_array ) );
    levels.pFeatureLevelsRequested = levels_array;

    hr = d3d12_device->CheckFeatureSupport( D3D12_FEATURE_FEATURE_LEVELS, &levels, sizeof( levels ) );
    if ( SUCCEEDED( hr ) && levels.MaxSupportedFeatureLevel == D3D_FEATURE_LEVEL_12_2 ) {
        log_info( L"DirectX 12 Ultimate Supported" );
    } else {
        log_info( L"DirectX 12 Ultimate Unsupported" );
    }

#ifdef _DEBUG
    ComPtr<ID3D12InfoQueue> info_queue{};
    hr = d3d12_device->QueryInterface( IID_PPV_ARGS( &info_queue ) );
    if ( FAILED( hr ) ) {
        log_hresult( hr );
    } else {
        hr = info_queue->SetBreakOnSeverity( D3D12_MESSAGE_SEVERITY_CORRUPTION, true );
        if ( FAILED( hr ) ) {
            log_hresult( hr );
        }
        hr = info_queue->SetBreakOnSeverity( D3D12_MESSAGE_SEVERITY_ERROR, true );
        if ( FAILED( hr ) ) {
            log_hresult( hr );
        }
        hr = info_queue->SetBreakOnSeverity( D3D12_MESSAGE_SEVERITY_WARNING, true );
        if ( FAILED( hr ) ) {
            log_hresult( hr );
        }
    }
#endif

    auto command_queue = CommandQueue::create( d3d12_device, D3D12_COMMAND_LIST_TYPE_DIRECT );

    if ( !command_queue ) {
        return nullptr;
    }

    auto window_class = WindowClass::create( L"MKSV WindowClass" );
    if ( !window_class ) {
        return nullptr;
    }

    auto window = Window::create(
        WindowProps{ .width = 800, .height = 600, .title = L"MKSV Engine", .class_name = window_class->get_name().data() },
        dxgi_factory,
        command_queue->get_ptr(),
        d3d12_device
    );

    if ( !window ) {
        return nullptr;
    }

    window->show();

    return std::unique_ptr<Engine>{ new Engine(
        h_instance,
        std::move( window_class ),
        std::move( window ),
        dxgi_adapter,
        d3d12_device,
        std::move( command_queue )
    ) };
}

Engine::Engine( Engine&& other )
    : h_instance_{ other.h_instance_ },
      window_class_{ std::move( other.window_class_ ) },
      window_{ std::move( other.window_ ) },
      keyboard_{ std::move( other.keyboard_ ) },
      adapter_{ std::move( other.adapter_ ) },
      device_{ std::move( other.device_ ) },
      command_queue_{ std::move( other.command_queue_ ) },
      vertex_buffer_{ std::move( other.vertex_buffer_ ) }
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
    keyboard_ = std::move( other.keyboard_ );
    adapter_ = std::move( other.adapter_ );
    device_ = std::move( other.device_ );
    command_queue_ = std::move( other.command_queue_ );
    vertex_buffer_ = std::move( other.vertex_buffer_ );
    other.h_instance_ = nullptr;

    return *this;
}

Engine::~Engine()
{
    command_queue_->flush();
    --instance_count;
}

auto Engine::copy_data() -> bool
{
    auto command_list = command_queue_->get_command_list();
    if ( !command_list ) {
        return false;
    }

    ID3D12CommandAllocator* command_allocator = nullptr;
    u32                     data_size = sizeof( command_allocator );
    HRESULT hr = command_list->GetPrivateData( __uuidof( ID3D12CommandAllocator ), &data_size, &command_allocator );

    if ( FAILED( hr ) ) {
        log_hresult( hr );
        return false;
    }

    const vec3 red = { 1.0f, 0.0f, 0.0f };
    const vec3 green = { 0.0f, 1.0f, 0.0f };
    const vec3 blue = { 0.0f, 0.0f, 1.0f };

    const vec3 top = { 0.00f, 0.50f, 0.0f };
    const vec3 right = { 0.43f, -0.25f, 0.0f };
    const vec3 left = { -0.43f, -0.25f, 0.0f };

    const Vertex vertices[] = {
        {top,    red  },
        { right, blue },
        { left,  green},
    };

    {
        const auto heap_props = d3d12::heap_properties( D3D12_HEAP_TYPE_DEFAULT );
        const auto res_desc = d3d12::buffer_resource_desc( sizeof( vertices ) );

        hr = device_->CreateCommittedResource(
            &heap_props,
            D3D12_HEAP_FLAG_CREATE_NOT_ZEROED,
            &res_desc,
            D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr,
            IID_PPV_ARGS( &vertex_buffer_ )
        );

        if ( FAILED( hr ) ) {
            log_hresult( hr );
            return false;
        }
    }

    ComPtr<ID3D12Resource> vertex_upload_buffer{};
    {
        const auto heap_props = d3d12::heap_properties( D3D12_HEAP_TYPE_UPLOAD );
        const auto res_desc = d3d12::buffer_resource_desc( sizeof( vertices ) );

        hr = device_->CreateCommittedResource(
            &heap_props,
            D3D12_HEAP_FLAG_CREATE_NOT_ZEROED,
            &res_desc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS( &vertex_upload_buffer )
        );

        if ( FAILED( hr ) ) {
            log_hresult( hr );
            return false;
        }
    }

    Vertex* mapped_vertices = nullptr;
    hr = vertex_upload_buffer->Map( 0, nullptr, reinterpret_cast<void**>( &mapped_vertices ) );

    if ( FAILED( hr ) ) {
        log_hresult( hr );
        return false;
    }

    std::ranges::copy( vertices, mapped_vertices );
    vertex_upload_buffer->Unmap( 0, nullptr );

    hr = command_allocator->Reset();
    if ( FAILED( hr ) ) {
        log_hresult( hr );
        return false;
    }

    hr = command_list->Reset( command_allocator, nullptr );
    if ( FAILED( hr ) ) {
        log_hresult( hr );
        return false;
    }

    command_list->CopyResource( vertex_buffer_.Get(), vertex_upload_buffer.Get() );
    hr = command_list->Close();
    if ( FAILED( hr ) ) {
        log_hresult( hr );
        return false;
    }

    command_queue_->execute( command_list.Get() );
    hr = command_queue_->flush();
    if ( FAILED( hr ) ) {
        log_hresult( hr );
        return false;
    }

    return true;
}

auto Engine::update() -> void
{
    using namespace std::chrono;

    static auto prev = high_resolution_clock::now();

    auto command_list = command_queue_->get_command_list();
    if ( !command_list ) {
        return;
    }

    ID3D12CommandAllocator* command_allocator = nullptr;
    u32                     data_size = sizeof( command_allocator );
    HRESULT hr = command_list->GetPrivateData( __uuidof( ID3D12CommandAllocator ), &data_size, &command_allocator );

    if ( FAILED( hr ) ) {
        log_hresult( hr );
        return;
    }

    const u32   current_index = window_->get_current_back_buffer_index();
    const auto& back_buffer = window_->get_back_buffer( current_index );

    hr = command_allocator->Reset();
    if ( FAILED( hr ) ) {
        log_hresult( hr );
        return;
    }

    hr = command_list->Reset( command_allocator, nullptr );
    if ( FAILED( hr ) ) {
        log_hresult( hr );
        return;
    }

    {
        const auto barrier =
            d3d12::transition_barrier( back_buffer.Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET );

        command_list->ResourceBarrier( 1, &barrier );
    }

    const auto now = high_resolution_clock::now();
    const f32  dt = duration_cast<duration<f32>>( now - prev ).count();
    prev = now;

    static f32 time = 0.0f;
    time += 1.0f * dt;
    if ( time >= 2.0f * PI ) {
        time -= 2.0f * PI;
    }

    const f32  r = 0.5f + 0.5f * sin( time + 1.0f );
    const f32  g = 0.5f + 0.5f * sin( time + 3.0f );
    const f32  b = 0.5f + 0.5f * sin( time + 6.0f );
    const f32  clear_color[4] = { r, g, b, 1.0f };
    const auto rtv = window_->get_render_target_view( current_index );
    command_list->ClearRenderTargetView( rtv, clear_color, 0, nullptr );

    {
        const auto barrier =
            d3d12::transition_barrier( back_buffer.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT );

        command_list->ResourceBarrier( 1, &barrier );
    }

    hr = command_list->Close();
    if ( FAILED( hr ) ) {
        log_hresult( hr );
        return;
    }

    command_queue_->execute( command_list.Get() );

    hr = window_->present( false );
    if ( FAILED( hr ) ) {
        log_hresult( hr );
        return;
    }

    hr = command_queue_->flush();
    if ( FAILED( hr ) ) {
        log_hresult( hr );
        return;
    }
}

Engine::Engine(
    const HINSTANCE               h_instance,
    std::unique_ptr<WindowClass>  window_class,
    std::unique_ptr<Window>       window,
    ComPtr<DXGIAdapter>           adapter,
    ComPtr<D3D12Device>           device,
    std::unique_ptr<CommandQueue> command_queue
)
    : h_instance_{ h_instance },
      window_class_{ std::move( window_class ) },
      window_{ std::move( window ) },
      adapter_{ std::move( adapter ) },
      device_{ std::move( device ) },
      command_queue_{ std::move( command_queue ) },
      vertex_buffer_{ nullptr }
{
    assert( instance_count == 0 && "Only 1 engine instance can exist at a time" );
    const LONG_PTR result = SetWindowLongPtrW( window_->handle(), GWLP_USERDATA, reinterpret_cast<LONG_PTR>( this ) );
    if ( result != 0 ) {
        log_last_window_error();
    }
    ++instance_count;
}

auto Engine::GetKeyboard() -> Keyboard&
{
    return keyboard_;
}

} // namespace mksv
