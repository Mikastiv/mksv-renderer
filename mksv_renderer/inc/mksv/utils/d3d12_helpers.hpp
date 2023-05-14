#pragma once

#include "mksv/common/types.hpp"

#include <d3d12.h>

namespace mksv::d3d12
{
auto transition_barrier( ID3D12Resource* resource, const D3D12_RESOURCE_STATES before, const D3D12_RESOURCE_STATES after )
    -> D3D12_RESOURCE_BARRIER;

auto heap_properties( const D3D12_HEAP_TYPE type ) -> D3D12_HEAP_PROPERTIES;

auto buffer_resource_desc( const u64 width ) -> D3D12_RESOURCE_DESC;
} // namespace mksv::d3d12
