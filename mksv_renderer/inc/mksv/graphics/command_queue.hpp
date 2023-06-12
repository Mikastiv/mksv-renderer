#pragma once

#include "mksv/common/types.hpp"
#include "mksv/mksv_d3d12.hpp"
#include "mksv/mksv_wrl.hpp"

#include <memory>

namespace mksv
{
class CommandQueue
{
public:
    static auto create( ComPtr<D3D12Device> device, const D3D12_COMMAND_LIST_TYPE type ) -> std::unique_ptr<CommandQueue>;

public:
    CommandQueue( const CommandQueue& ) = delete;
    CommandQueue( CommandQueue&& ) = default;
    auto operator=( const CommandQueue& ) -> CommandQueue& = delete;
    auto operator=( CommandQueue&& ) -> CommandQueue& = default;
    ~CommandQueue();

public:
    auto get_ptr() const -> ComPtr<ID3D12CommandQueue>;
    auto execute( ID3D12CommandList* const command_list ) -> void;
    auto signal() -> u64;
    auto is_fence_complete( const u64 fence_value ) const -> bool;
    auto wait_for_fence_value( const u64 fence_value ) -> HRESULT;
    auto flush() -> HRESULT;

private:
    CommandQueue( ComPtr<ID3D12CommandQueue> queue, ComPtr<ID3D12Fence> fence, const HANDLE event );

private:
    ComPtr<ID3D12CommandQueue> queue_;
    ComPtr<ID3D12Fence>        fence_;
    HANDLE                     fence_event_;
    u64                        fence_value_;
};

} // namespace mksv
