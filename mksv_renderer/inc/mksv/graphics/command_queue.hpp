#pragma once

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
    ~CommandQueue() = default;

public:
    auto get_ptr() const -> ComPtr<D3D12CommandQueue>;

private:
    CommandQueue( ComPtr<D3D12Device> device, ComPtr<D3D12CommandQueue> queue, ComPtr<D3D12Fence> fence, const HANDLE event );

private:
    ComPtr<D3D12Device>       device_;
    ComPtr<D3D12CommandQueue> queue_;
    ComPtr<D3D12Fence>        fence_;
    HANDLE                    fence_event_;
};

} // namespace mksv
