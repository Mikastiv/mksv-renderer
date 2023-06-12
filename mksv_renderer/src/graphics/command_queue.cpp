#include "mksv/graphics/command_queue.hpp"

#include "mksv/log.hpp"
#include "mksv/mksv_d3d12.hpp"

#include <utility>

namespace mksv
{

auto CommandQueue::create( ComPtr<D3D12Device> device, const D3D12_COMMAND_LIST_TYPE type )
    -> std::unique_ptr<CommandQueue>
{

    const HANDLE event = CreateEventW( nullptr, false, false, nullptr );
    if ( !event ) {
        log_last_window_error();
        return nullptr;
    }

    ComPtr<ID3D12CommandQueue>     command_queue{};
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

    ComPtr<ID3D12Fence> fence{};
    hr = device->CreateFence( 0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS( &fence ) );

    if ( FAILED( hr ) ) {
        log_hresult( hr );
        return nullptr;
    }

    return std::unique_ptr<CommandQueue>( new CommandQueue( std::move( command_queue ), std::move( fence ), event ) );
}

CommandQueue::~CommandQueue()
{
    CloseHandle( fence_event_ );
}

CommandQueue::CommandQueue( ComPtr<ID3D12CommandQueue> queue, ComPtr<ID3D12Fence> fence, const HANDLE event )
    : queue_{ std::move( queue ) },
      fence_{ std::move( fence ) },
      fence_event_{ event },
      fence_value_{ 0 }
{
}

auto CommandQueue::get_ptr() const -> ComPtr<ID3D12CommandQueue>
{
    return queue_;
}

auto CommandQueue::execute( ID3D12CommandList* const command_list ) -> void
{
    queue_->ExecuteCommandLists( 1, &command_list );
}

auto CommandQueue::signal() -> u64
{
    ++fence_value_;
    queue_->Signal( fence_.Get(), fence_value_ );
    return fence_value_;
}

auto CommandQueue::is_fence_complete( const u64 fence_value ) const -> bool
{
    return fence_->GetCompletedValue() >= fence_value;
}

auto CommandQueue::wait_for_fence_value( const u64 fence_value ) -> HRESULT
{
    if ( is_fence_complete( fence_value ) ) {
        return S_OK;
    }

    const HRESULT hr = fence_->SetEventOnCompletion( fence_value, fence_event_ );
    if ( FAILED( hr ) ) {
        return hr;
    }

    if ( WaitForSingleObject( fence_event_, DWORD_MAX ) == WAIT_FAILED ) {
        log_last_window_error();
        return E_FAIL;
    }

    return S_OK;
}

auto CommandQueue::flush() -> HRESULT
{
    return wait_for_fence_value( signal() );
}

} // namespace mksv
