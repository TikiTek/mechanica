
#include "tiki/graphics/samplerstate.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/functions.hpp"
#include "tiki/graphics/graphicssystem.hpp"

#include "graphicssystem_internal_d3d12.hpp"

namespace tiki
{
	static const D3D12_TEXTURE_ADDRESS_MODE s_aAddressModeMapping[] =
	{
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,	// AddressMode_Wrap
		D3D12_TEXTURE_ADDRESS_MODE_MIRROR,	// AddressMode_Mirror
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,	// AddressMode_Clamp
		D3D12_TEXTURE_ADDRESS_MODE_BORDER,	// AddressMode_Border
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aAddressModeMapping ) == AddressMode_Count );

	static D3D12_FILTER getFilter( FilterMode mag, FilterMode mip )
	{
		switch ( mag )
		{
		case FilterMode_Anisotropic:
			TIKI_ASSERT( mip == FilterMode_Anisotropic );
			return D3D12_FILTER_ANISOTROPIC;
		case FilterMode_Linear:
			{
				TIKI_ASSERT( mip != FilterMode_Anisotropic );
				switch ( mip )
				{
				case FilterMode_Linear:
					return D3D12_FILTER_MIN_MAG_MIP_LINEAR;
				case FilterMode_Nearest:
					return D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT;
				}
			}
			break;
		case FilterMode_Nearest:
			{
				TIKI_ASSERT( mip != FilterMode_Anisotropic );
				switch ( mip )
				{
				case FilterMode_Linear:
					return D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR;
				case  FilterMode_Nearest:
					return D3D12_FILTER_MIN_MAG_MIP_POINT;
				}
			}
			break;
		}

		TIKI_BREAK( "[graphics] wrong FilterMode.\n" );
		return (D3D12_FILTER)0;
	}

	bool SamplerState::isCreated() const
	{
		return m_platformData.pDescriptorHeap != nullptr;
	}

	bool SamplerState::create( GraphicsSystem& graphicsSystem, const SamplerStateParamters& creationParamter )
	{
		ID3D12Device* pDevice = GraphicsSystemPlatform::getDevice( graphicsSystem );

		TIKI_DECLARE_STACKANDZERO( D3D12_SAMPLER_DESC, samplerDesc );
		samplerDesc.Filter				= getFilter( creationParamter.magFilter, creationParamter.mipFilter );
		samplerDesc.AddressU			= s_aAddressModeMapping[ creationParamter.addressU ];
		samplerDesc.AddressV			= s_aAddressModeMapping[ creationParamter.addressV ];
		samplerDesc.AddressW			= s_aAddressModeMapping[ creationParamter.addressW ];
		samplerDesc.MipLODBias			= 0.0f;
		samplerDesc.MaxAnisotropy		= uint32( creationParamter.maxAnisotropy );
		samplerDesc.ComparisonFunc		= D3D12_COMPARISON_FUNC_ALWAYS;
		samplerDesc.BorderColor[ 0 ]	= color::getFloatChannelR( creationParamter.borderColor );
		samplerDesc.BorderColor[ 1 ]	= color::getFloatChannelG( creationParamter.borderColor );
		samplerDesc.BorderColor[ 2 ]	= color::getFloatChannelB( creationParamter.borderColor );
		samplerDesc.BorderColor[ 3 ]	= color::getFloatChannelA( creationParamter.borderColor );
		samplerDesc.MinLOD				= 0;
		samplerDesc.MaxLOD				= D3D12_FLOAT32_MAX;

		TIKI_DECLARE_STACKANDZERO( D3D12_DESCRIPTOR_HEAP_DESC, heapDesc );
		heapDesc.NumDescriptors	= 1u;
		heapDesc.Type			= D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
		heapDesc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

		const HRESULT result = pDevice->CreateDescriptorHeap( &heapDesc, IID_PPV_ARGS( &m_platformData.pDescriptorHeap ) );
		if( FAILED( result ) )
		{
			dispose( graphicsSystem );
			return false;
		}

		pDevice->CreateSampler( &samplerDesc, m_platformData.pDescriptorHeap->GetCPUDescriptorHandleForHeapStart() );

		return true;
	}

	void SamplerState::dispose( GraphicsSystem& /*graphicsSystem*/ )
	{
		GraphicsSystemPlatform::safeRelease( &m_platformData.pDescriptorHeap );
		
		GraphicsStateObject::dispose();
	}
}