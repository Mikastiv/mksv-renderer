#include "mksv/engine.hpp"

#include "mksv/common/types.hpp"
#include "mksv/log.hpp"
#include "mksv/math/consts.hpp"
#include "mksv/math/types.hpp"
#include "mksv/utils/d3d12_helpers.hpp"
#include "mksv/utils/helpers.hpp"
#include "mksv/utils/string.hpp"

#include <cassert>
#include <chrono>
#include <cmath>
#include <d3dcompiler.h>
#include <ranges>

namespace DX = DirectX;

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
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_2,
    };
    levels.NumFeatureLevels = static_cast<u32>( std::size( levels_array ) );
    levels.pFeatureLevelsRequested = levels_array;

    hr = d3d12_device->CheckFeatureSupport( D3D12_FEATURE_FEATURE_LEVELS, &levels, sizeof( levels ) );
    if ( SUCCEEDED( hr ) ) {
        const wchar_t* feature_level_str = nullptr;
        switch ( levels.MaxSupportedFeatureLevel ) {
            case D3D_FEATURE_LEVEL_12_0:
                feature_level_str = L"12.0";
                break;
            case D3D_FEATURE_LEVEL_12_1:
                feature_level_str = L"12.1";
                break;
            case D3D_FEATURE_LEVEL_12_2:
                feature_level_str = L"12.2";
                break;
        }

        log_info( std::format( L"Supported feature level: {}", feature_level_str ) );
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
      command_allocator_{ std::move( other.command_allocator_ ) },
      command_list_{ std::move( other.command_list_ ) },
      vertex_buffer_{ std::move( other.vertex_buffer_ ) },
      vertex_buffer_view_{ other.vertex_buffer_view_ },
      index_buffer_{ std::move( other.index_buffer_ ) },
      index_buffer_view_{ other.index_buffer_view_ },
      root_signature_{ std::move( other.root_signature_ ) },
      pipeline_state_{ std::move( other.pipeline_state_ ) }
{
    other.h_instance_ = nullptr;
    other.vertex_buffer_view_ = {};
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
    command_allocator_ = std::move( other.command_allocator_ );
    command_list_ = std::move( other.command_list_ );
    vertex_buffer_ = std::move( other.vertex_buffer_ );
    vertex_buffer_view_ = other.vertex_buffer_view_;
    index_buffer_ = std::move( other.index_buffer_ );
    index_buffer_view_ = other.index_buffer_view_;
    root_signature_ = std::move( other.root_signature_ );
    pipeline_state_ = std::move( other.pipeline_state_ );
    other.h_instance_ = nullptr;
    other.vertex_buffer_view_ = {};

    return *this;
}

Engine::~Engine()
{
    command_queue_->flush();
    --instance_count;
}

auto Engine::init() -> bool
{
    const auto desc = command_queue_->get_ptr()->GetDesc();

    HRESULT hr = device_->CreateCommandAllocator( desc.Type, IID_PPV_ARGS( &command_allocator_ ) );

    if ( FAILED( hr ) ) {
        log_hresult( hr );
        return false;
    }

    hr = device_->CreateCommandList1( 0, desc.Type, D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS( &command_list_ ) );

    if ( FAILED( hr ) ) {
        log_hresult( hr );
        return false;
    }

    return true;
}

auto Engine::copy_data() -> bool
{
    const vec3 red = { 1.0f, 0.0f, 0.0f };
    const vec3 yellow = { 1.0f, 1.0f, 0.0f };
    const vec3 green = { 0.0f, 1.0f, 0.0f };
    const vec3 cyan = { 0.0f, 1.0f, 1.0f };
    const vec3 blue = { 0.0f, 0.0f, 1.0f };

    const Vertex vertices[] = {
        {{ -0.5f, 0.5f, 0.5f },    red   },
        { { 0.5f, 0.5f, 0.5f },    yellow},
        { { 0.5f, -0.5f, 0.5f },   blue  },
        { { -0.5f, -0.5f, 0.5f },  green },
        { { -0.5f, 0.5f, -0.5f },  cyan  },
        { { 0.5f, 0.5f, -0.5f },   green },
        { { 0.5f, -0.5f, -0.5f },  red   },
        { { -0.5f, -0.5f, -0.5f }, yellow},
    };

    const u32 indices[] = {
        0, 2, 1, 2, 0, 3, // front
        0, 5, 4, 5, 0, 1, // top
        3, 6, 2, 6, 3, 7, // botttom
        4, 6, 7, 6, 4, 5, // back
        0, 7, 3, 7, 0, 4, // left
        1, 6, 5, 6, 1, 2, // right
    };

    HRESULT hr = E_FAIL;

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

    {
        Vertex* mapped_vertices = nullptr;
        hr = vertex_upload_buffer->Map( 0, nullptr, reinterpret_cast<void**>( &mapped_vertices ) );

        if ( FAILED( hr ) ) {
            log_hresult( hr );
            return false;
        }

        std::ranges::copy( vertices, mapped_vertices );
        vertex_upload_buffer->Unmap( 0, nullptr );
    }

    {
        const auto heap_props = d3d12::heap_properties( D3D12_HEAP_TYPE_DEFAULT );
        const auto res_desc = d3d12::buffer_resource_desc( sizeof( indices ) );

        hr = device_->CreateCommittedResource(
            &heap_props,
            D3D12_HEAP_FLAG_CREATE_NOT_ZEROED,
            &res_desc,
            D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr,
            IID_PPV_ARGS( &index_buffer_ )
        );

        if ( FAILED( hr ) ) {
            log_hresult( hr );
            return false;
        }
    }

    ComPtr<ID3D12Resource> index_upload_buffer{};
    {
        const auto heap_props = d3d12::heap_properties( D3D12_HEAP_TYPE_UPLOAD );
        const auto res_desc = d3d12::buffer_resource_desc( sizeof( indices ) );

        hr = device_->CreateCommittedResource(
            &heap_props,
            D3D12_HEAP_FLAG_CREATE_NOT_ZEROED,
            &res_desc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS( &index_upload_buffer )
        );
    }

    {
        u32* mapped_indices = nullptr;
        hr = index_upload_buffer->Map( 0, nullptr, reinterpret_cast<void**>( &mapped_indices ) );

        if ( FAILED( hr ) ) {
            log_hresult( hr );
            return false;
        }

        std::ranges::copy( indices, mapped_indices );
        index_upload_buffer->Unmap( 0, nullptr );
    }

    hr = command_allocator_->Reset();
    if ( FAILED( hr ) ) {
        log_hresult( hr );
        return false;
    }

    hr = command_list_->Reset( command_allocator_.Get(), nullptr );
    if ( FAILED( hr ) ) {
        log_hresult( hr );
        return false;
    }

    command_list_->CopyResource( vertex_buffer_.Get(), vertex_upload_buffer.Get() );
    const auto vertex_barrier = d3d12::transition_barrier(
        vertex_buffer_.Get(),
        D3D12_RESOURCE_STATE_COPY_DEST,
        D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER
    );
    command_list_->ResourceBarrier( 1, &vertex_barrier );

    command_list_->CopyResource( index_buffer_.Get(), index_upload_buffer.Get() );
    const auto index_barrier =
        d3d12::transition_barrier( index_buffer_.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER );
    command_list_->ResourceBarrier( 1, &index_barrier );

    hr = command_list_->Close();
    if ( FAILED( hr ) ) {
        log_hresult( hr );
        return false;
    }

    command_queue_->execute( command_list_.Get() );
    hr = command_queue_->flush();
    if ( FAILED( hr ) ) {
        log_hresult( hr );
        return false;
    }

    vertex_buffer_view_ = {
        .BufferLocation = vertex_buffer_->GetGPUVirtualAddress(),
        .SizeInBytes = sizeof( vertices ),
        .StrideInBytes = sizeof( Vertex ),
    };

    index_buffer_view_ = {
        .BufferLocation = index_buffer_->GetGPUVirtualAddress(),
        .SizeInBytes = sizeof( indices ),
        .Format = DXGI_FORMAT_R32_UINT,
    };

    const D3D12_ROOT_PARAMETER params[1] = {
        d3d12::create_root_constant( sizeof( mat4 ) / sizeof( u32 ), 0, 0, D3D12_SHADER_VISIBILITY_VERTEX ),
    };

    const D3D12_ROOT_SIGNATURE_DESC root_signature_desc = {
        .NumParameters = 1,
        .pParameters = params,
        .NumStaticSamplers = 0,
        .pStaticSamplers = nullptr,
        .Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
                 D3D12_ROOT_SIGNATURE_FLAG_DENY_AMPLIFICATION_SHADER_ROOT_ACCESS |
                 D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
                 D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
                 D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
                 D3D12_ROOT_SIGNATURE_FLAG_DENY_MESH_SHADER_ROOT_ACCESS |
                 D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS,
    };

    ComPtr<ID3DBlob> signature_blob;
    ComPtr<ID3DBlob> error_blob;
    hr = D3D12SerializeRootSignature( &root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature_blob, &error_blob );
    if ( FAILED( hr ) ) {
        log_hresult( hr );
        if ( error_blob ) {
            const auto error_msg = static_cast<const char*>( error_blob->GetBufferPointer() );
            log_error( string_to_wstring( error_msg ) );
        }
        return false;
    }

    hr = device_->CreateRootSignature(
        0,
        signature_blob->GetBufferPointer(),
        signature_blob->GetBufferSize(),
        IID_PPV_ARGS( &root_signature_ )
    );
    if ( FAILED( hr ) ) {
        log_hresult( hr );
        return false;
    }

    struct PipelineStateStream {
        d3d12::PSSRootSignature       root_sig;
        d3d12::PSSInputLayout         input_layout;
        d3d12::PSSPrimitiveTopology   primitive_topology;
        d3d12::PSSVertexShader        vs;
        d3d12::PSSPixelShader         ps;
        d3d12::PSSRenderTargetFormats rtv_formats;
    } pss;

    const D3D12_INPUT_ELEMENT_DESC elements_desc[] = {
        {
         .SemanticName = "POSITION",
         .SemanticIndex = 0,
         .Format = DXGI_FORMAT_R32G32B32_FLOAT,
         .InputSlot = 0,
         .AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT,
         .InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
         .InstanceDataStepRate = 0,
         },
        {
         .SemanticName = "COLOR",
         .SemanticIndex = 0,
         .Format = DXGI_FORMAT_R32G32B32_FLOAT,
         .InputSlot = 0,
         .AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT,
         .InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
         .InstanceDataStepRate = 0,
         },
    };
    const D3D12_INPUT_LAYOUT_DESC input_layout = {
        .pInputElementDescs = elements_desc,
        .NumElements = static_cast<UINT>( std::size( elements_desc ) ),
    };

    ComPtr<ID3DBlob> vs_blob;
    hr = D3DReadFileToBlob( L"vertex_shader.cso", &vs_blob );
    if ( FAILED( hr ) ) {
        log_hresult( hr );
        return false;
    }
    const D3D12_SHADER_BYTECODE vs_bytecode = {
        .pShaderBytecode = vs_blob->GetBufferPointer(),
        .BytecodeLength = vs_blob->GetBufferSize(),
    };

    ComPtr<ID3DBlob> ps_blob;
    hr = D3DReadFileToBlob( L"pixel_shader.cso", &ps_blob );
    if ( FAILED( hr ) ) {
        log_hresult( hr );
        return false;
    }

    const D3D12_SHADER_BYTECODE ps_bytecode = {
        .pShaderBytecode = ps_blob->GetBufferPointer(),
        .BytecodeLength = ps_blob->GetBufferSize(),
    };

    pss.root_sig = root_signature_.Get();
    pss.input_layout = input_layout;
    pss.primitive_topology = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    pss.vs = vs_bytecode;
    pss.ps = ps_bytecode;
    pss.rtv_formats = {
        .RTFormats{ DXGI_FORMAT_R8G8B8A8_UNORM },
        .NumRenderTargets = 1,
    };

    const D3D12_PIPELINE_STATE_STREAM_DESC pss_desc = {
        .SizeInBytes = sizeof( pss ),
        .pPipelineStateSubobjectStream = &pss,
    };
    hr = device_->CreatePipelineState( &pss_desc, IID_PPV_ARGS( &pipeline_state_ ) );
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

    const u32   current_index = window_->get_current_back_buffer_index();
    const auto& back_buffer = window_->get_back_buffer( current_index );

    HRESULT hr = command_allocator_->Reset();
    if ( FAILED( hr ) ) {
        log_hresult( hr );
        return;
    }

    hr = command_list_->Reset( command_allocator_.Get(), nullptr );
    if ( FAILED( hr ) ) {
        log_hresult( hr );
        return;
    }

    {
        const auto barrier =
            d3d12::transition_barrier( back_buffer.Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET );

        command_list_->ResourceBarrier( 1, &barrier );
    }

    const auto now = high_resolution_clock::now();
    const f32  dt = duration_cast<duration<f32>>( now - prev ).count();
    prev = now;

    static f32 angle = 0.0f;
    angle += 1.0f * dt;
    if ( angle >= 2.0f * PI ) {
        angle -= 2.0f * PI;
    }

    const f32  r = 0.5f + 0.5f * sin( angle + 1.0f );
    const f32  g = 0.5f + 0.5f * sin( angle + 3.0f );
    const f32  b = 0.5f + 0.5f * sin( angle + 6.0f );
    const f32  clear_color[4] = { r, g, b, 1.0f };
    const auto rtv = window_->get_render_target_view( current_index );
    command_list_->ClearRenderTargetView( rtv, clear_color, 0, nullptr );

    const D3D12_RECT scissor_rect = {
        .left = 0,
        .top = 0,
        .right = LONG_MAX,
        .bottom = LONG_MAX,
    };

    const D3D12_VIEWPORT viewport = {
        .TopLeftX = 0.0f,
        .TopLeftY = 0.0f,
        .Width = static_cast<f32>( window_->width() ),
        .Height = static_cast<f32>( window_->height() ),
        .MinDepth = D3D12_MIN_DEPTH,
        .MaxDepth = D3D12_MAX_DEPTH,
    };

    const vec4 axis = DX::XMVectorSet( 0, 1, 1, 0 );
    const mat4 rotation = DX::XMMatrixRotationAxis( axis, angle );
    const vec4 eye_pos = DX::XMVectorSet( 0.0f, 0.0f, -2.0f, 1.0f );
    const vec4 focus_pos = DX::XMVectorSet( 0.0f, 0.0f, 0.0f, 1.0f );
    const vec4 up = DX::XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
    const mat4 view = DX::XMMatrixLookAtLH( eye_pos, focus_pos, up );
    const f32  aspect_ratio = static_cast<f32>( window_->width() ) / static_cast<f32>( window_->height() );
    const mat4 projection = DX::XMMatrixPerspectiveFovLH( DX::XMConvertToRadians( 80.0f ), aspect_ratio, 0.1f, 100.0f );
    const mat4 mvp = DX::XMMatrixTranspose( rotation * view * projection );

    command_list_->SetPipelineState( pipeline_state_.Get() );
    command_list_->SetGraphicsRootSignature( root_signature_.Get() );
    command_list_->SetGraphicsRoot32BitConstants( 0, sizeof( mat4 ) / sizeof( u32 ), &mvp, 0 );
    command_list_->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
    command_list_->IASetVertexBuffers( 0, 1, &vertex_buffer_view_ );
    command_list_->IASetIndexBuffer( &index_buffer_view_ );
    command_list_->RSSetViewports( 1, &viewport );
    command_list_->RSSetScissorRects( 1, &scissor_rect );
    command_list_->OMSetRenderTargets( 1, &rtv, true, nullptr );
    command_list_->DrawIndexedInstanced( index_buffer_view_.SizeInBytes / sizeof( u32 ), 1, 0, 0, 0 );

    {
        const auto barrier =
            d3d12::transition_barrier( back_buffer.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT );

        command_list_->ResourceBarrier( 1, &barrier );
    }

    hr = command_list_->Close();
    if ( FAILED( hr ) ) {
        log_hresult( hr );
        return;
    }

    command_queue_->execute( command_list_.Get() );

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
