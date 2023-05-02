#include "mksv/graphics/command_queue.hpp"

#include "mksv/log.hpp"
#include "mksv/mksv_d3d12.hpp"

#include <utility>

namespace mksv
{

CommandQueue::CommandQueue(
    ComPtr<D3D12Device>       device,
    ComPtr<D3D12CommandQueue> queue,
    ComPtr<D3D12Fence>        fence,
    const HANDLE              event
)
    : device_{ std::move( device ) },
      queue_{ std::move( queue ) },
      fence_{ std::move( fence ) },
      fence_event_{ event }
{
}

auto create_command_queue( ComPtr<D3D12Device> device, const D3D12_COMMAND_LIST_TYPE type )
    -> std::unique_ptr<CommandQueue>
{

    const HANDLE event = CreateEventW( nullptr, false, false, nullptr );
    if ( !event ) {
        log_last_window_error();
        return nullptr;
    }

    ComPtr<D3D12CommandQueue>      command_queue{};
    const D3D12_COMMAND_QUEUE_DESC desc{
        .Type = type,
        .Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
        .Flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
        .NodeMask = 0,
    };
    HRESULT hr = device->CreateCommandQueue( &desc, IID_PPV_ARGS( &command_queue ) );

    if ( FAILED( hr ) ) {
        log_hresult( hr );
        return nullptr;
    }

    ComPtr<D3D12Fence> fence{};
    hr = device->CreateFence( 0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS( &fence ) );

    if ( FAILED( hr ) ) {
        log_hresult( hr );
        return nullptr;
    }

    return std::unique_ptr<CommandQueue>(
        new CommandQueue( std::move( device ), std::move( command_queue ), std::move( fence ), event )
    );
}
} // namespace mksv
