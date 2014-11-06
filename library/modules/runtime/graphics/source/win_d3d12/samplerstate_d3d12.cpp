
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
		D3D12_TEXTURE_ADDRESS_WRAP,		// AddressMode_Wrap
		D3D12_TEXTURE_ADDRESS_MIRROR,	// AddressMode_Mirror
		D3D12_TEXTURE_ADDRESS_CLAMP,	// AddressMode_Clamp
		D3D12_TEXTURE_ADDRESS_BORDER,	// AddressMode_Border
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
		return m_platformData.isCreated;
	}

	bool SamplerState::create( GraphicsSystem& graphicsSystem, const SamplerStateParamters& creationParamter )
	{
		memory::zero( m_platformData.samplerDesc );
		m_platformData.samplerDesc.Filter			= getFilter( creationParamter.magFilter, creationParamter.mipFilter );
		m_platformData.samplerDesc.AddressU			= s_aAddressModeMapping[ creationParamter.addressU ];
		m_platformData.samplerDesc.AddressV			= s_aAddressModeMapping[ creationParamter.addressV ];
		m_platformData.samplerDesc.AddressW			= s_aAddressModeMapping[ creationParamter.addressW ];
		m_platformData.samplerDesc.MipLODBias		= 0.0f;
		m_platformData.samplerDesc.MaxAnisotropy	= uint32( creationParamter.maxAnisotropy );
		m_platformData.samplerDesc.ComparisonFunc	= D3D12_COMPARISON_ALWAYS;
		m_platformData.samplerDesc.BorderColor[ 0 ]	= color::getFloatChannelR( creationParamter.borderColor );
		m_platformData.samplerDesc.BorderColor[ 1 ]	= color::getFloatChannelG( creationParamter.borderColor );
		m_platformData.samplerDesc.BorderColor[ 2 ]	= color::getFloatChannelB( creationParamter.borderColor );
		m_platformData.samplerDesc.BorderColor[ 3 ]	= color::getFloatChannelA( creationParamter.borderColor );
		m_platformData.samplerDesc.MinLOD			= 0;
		m_platformData.samplerDesc.MaxLOD			= D3D11_FLOAT32_MAX;

		m_platformData.isCreated = true;

		return true;
	}

	void SamplerState::dispose( GraphicsSystem& /*graphicsSystem*/ )
	{
		m_platformData.isCreated = false;
		GraphicsStateObject::dispose();
	}
}