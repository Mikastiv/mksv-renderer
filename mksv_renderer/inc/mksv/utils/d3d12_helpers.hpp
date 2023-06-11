#pragma once

#include "mksv/common/types.hpp"

#include <d3d12.h>

namespace mksv::d3d12
{
auto transition_barrier( ID3D12Resource* resource, const D3D12_RESOURCE_STATES before, const D3D12_RESOURCE_STATES after )
    -> D3D12_RESOURCE_BARRIER;

auto heap_properties( const D3D12_HEAP_TYPE type ) -> D3D12_HEAP_PROPERTIES;

auto buffer_resource_desc( const u64 width ) -> D3D12_RESOURCE_DESC;

auto create_root_constant(
    const u32                     num_32bits_values,
    const u32                     shader_register,
    const u32                     register_space = 0,
    const D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL
) -> D3D12_ROOT_PARAMETER;

template <typename Inner, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE type>
struct alignas( void* ) PSSSubobject {
public:
    PSSSubobject()
        : type_{ type },
          inner_{} {};
    PSSSubobject( const Inner& i )
        : type_{ type },
          inner_{ i } {};

    PSSSubobject& operator=( const Inner& i )
    {
        inner_ = i;
        return *this;
    }

private:
    D3D12_PIPELINE_STATE_SUBOBJECT_TYPE type_;
    Inner                               inner_;
};

using PSSRootSignature = PSSSubobject<ID3D12RootSignature*, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_ROOT_SIGNATURE>;
using PSSInputLayout = PSSSubobject<D3D12_INPUT_LAYOUT_DESC, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_INPUT_LAYOUT>;
using PSSPrimitiveTopology =
    PSSSubobject<D3D12_PRIMITIVE_TOPOLOGY_TYPE, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_PRIMITIVE_TOPOLOGY>;
using PSSVertexShader = PSSSubobject<D3D12_SHADER_BYTECODE, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_VS>;
using PSSPixelShader = PSSSubobject<D3D12_SHADER_BYTECODE, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_PS>;
using PSSRenderTargetFormats =
    PSSSubobject<D3D12_RT_FORMAT_ARRAY, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_RENDER_TARGET_FORMATS>;
} // namespace mksv::d3d12
