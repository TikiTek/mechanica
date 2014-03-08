
#include "tiki/graphics/samplerstate.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/functions.hpp"
#include "tiki/graphics/graphicssystem.hpp"

#include "graphicssystem_internal_d3d11.hpp"

namespace tiki
{
	static const D3D11_TEXTURE_ADDRESS_MODE s_aAddressModeMapping[ AddressMode_Count ] =
	{
		D3D11_TEXTURE_ADDRESS_WRAP,		// AddressMode_Wrap
		D3D11_TEXTURE_ADDRESS_MIRROR,	// AddressMode_Mirror
		D3D11_TEXTURE_ADDRESS_CLAMP,	// AddressMode_Clamp
		D3D11_TEXTURE_ADDRESS_BORDER,	// AddressMode_Border
	};

	static D3D11_FILTER getFilter( FilterMode mag, FilterMode mip )
	{
		switch ( mag )
		{
		case FilterMode_Anisotropic:
			TIKI_ASSERT( mip == FilterMode_Anisotropic );
			return D3D11_FILTER_ANISOTROPIC;
		case FilterMode_Linear:
			{
				TIKI_ASSERT( mip != FilterMode_Anisotropic );
				switch ( mip )
				{
				case FilterMode_Linear:
					return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				case FilterMode_Neares:
					return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
				}
			}
			break;
		case FilterMode_Neares:
			{
				TIKI_ASSERT( mip != FilterMode_Anisotropic );
				switch ( mip )
				{
				case FilterMode_Linear:
					return D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
				case  FilterMode_Neares:
					return D3D11_FILTER_MIN_MAG_MIP_POINT;
				}
			}
			break;
		}

		TIKI_BREAK( "[graphics] wrong FilterMode.\n" );
		return D3D11_FILTER_COMPARISON_ANISOTROPIC;
	}

	bool SamplerState::isCreated() const
	{
		return m_platformData.pSamplerState != nullptr;
	}

	bool SamplerState::create( GraphicsSystem& graphicsSystem, const SamplerStateParamters& creationParamter )
	{
		TIKI_DECLARE_STACKANDZERO( D3D11_SAMPLER_DESC, stateDesc );
		stateDesc.Filter			= getFilter( creationParamter.magFilter, creationParamter.mipFilter );
		stateDesc.AddressU			= s_aAddressModeMapping[ creationParamter.addressU ];
		stateDesc.AddressV			= s_aAddressModeMapping[ creationParamter.addressV ];
		stateDesc.AddressW			= s_aAddressModeMapping[ creationParamter.addressW ];
		stateDesc.MipLODBias		= 0.0f;
		stateDesc.MaxAnisotropy		= creationParamter.maxAnisotropy;
		stateDesc.ComparisonFunc	= D3D11_COMPARISON_ALWAYS;
		stateDesc.BorderColor[0]	= color::getFloatChannelR( creationParamter.borderColor );
		stateDesc.BorderColor[1]	= color::getFloatChannelG( creationParamter.borderColor );
		stateDesc.BorderColor[2]	= color::getFloatChannelB( creationParamter.borderColor );
		stateDesc.BorderColor[3]	= color::getFloatChannelA( creationParamter.borderColor );
		stateDesc.MinLOD			= 0;
		stateDesc.MaxLOD			= D3D11_FLOAT32_MAX;

		HRESULT result = graphics::getDevice( graphicsSystem )->CreateSamplerState( &stateDesc, &m_platformData.pSamplerState );
		if( FAILED( result ) )
		{
			dispose();
			return false;
		}

		return m_platformData.pSamplerState != nullptr;
	}

	void SamplerState::dispose()
	{
		if ( m_platformData.pSamplerState != nullptr )
		{
			m_platformData.pSamplerState->Release();
			m_platformData.pSamplerState = nullptr;
		}

		GraphicsStateObject::dispose();
	}
}