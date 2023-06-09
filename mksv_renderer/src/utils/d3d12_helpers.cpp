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
        .Transition = transition,
    };
}

auto heap_properties( const D3D12_HEAP_TYPE type ) -> D3D12_HEAP_PROPERTIES
{
    return {
        .Type = type,
        .CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
        .MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN,
        .CreationNodeMask = 1,
        .VisibleNodeMask = 1,
    };
}

auto buffer_resource_desc( const u64 width ) -> D3D12_RESOURCE_DESC
{
    const DXGI_SAMPLE_DESC sampleDesc = {
        .Count = 1,
        .Quality = 0,
    };

    return {
        .Dimension = D3D12_RESOURCE_DIMENSION_BUFFER,
        .Alignment = 0,
        .Width = width,
        .Height = 1,
        .DepthOrArraySize = 1,
        .MipLevels = 1,
        .Format = DXGI_FORMAT_UNKNOWN,
        .SampleDesc = sampleDesc,
        .Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
        .Flags = D3D12_RESOURCE_FLAG_NONE,
    };
}

auto create_root_constant(
    const u32                     num_32bits_values,
    const u32                     shader_register,
    const u32                     register_space,
    const D3D12_SHADER_VISIBILITY visibility
) -> D3D12_ROOT_PARAMETER
{
    const D3D12_ROOT_CONSTANTS constants = {
        .ShaderRegister = shader_register,
        .RegisterSpace = register_space,
        .Num32BitValues = num_32bits_values,
    };

    return D3D12_ROOT_PARAMETER{
        .ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS,
        .Constants = constants,
        .ShaderVisibility = visibility,
    };
}

} // namespace mksv::d3d12
