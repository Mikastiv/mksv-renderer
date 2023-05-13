#include "mksv/utils/d3d12_helpers.hpp"

namespace mksv::d3d12
{
auto transition_barrier( ID3D12Resource* resource, const D3D12_RESOURCE_STATES before, const D3D12_RESOURCE_STATES after )
    -> D3D12_RESOURCE_BARRIER
{
    const D3D12_RESOURCE_TRANSITION_BARRIER transition = {
        .pResource = resource,
        .Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
        .StateBefore = before,
        .StateAfter = after,
    };

    return {
        .Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
        .Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
        .Transition = transition };
}
} // namespace mksv::d3d12
