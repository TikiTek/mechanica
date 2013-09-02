
#include "tiki/graphics/samplerstate.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/functions.hpp"
#include "tiki/framework/framework.hpp"
#include "tiki/graphics/graphicssystem.hpp"

#include "graphicshandles.hpp"

#include <d3d11.h>

namespace tiki
{
	static D3D11_TEXTURE_ADDRESS_MODE getD3dAddressMode( AddressMode mode )
	{
		switch ( mode )
		{
		case AddressMode_Border:
			return D3D11_TEXTURE_ADDRESS_BORDER;
		case AddressMode_Clamp:
			return D3D11_TEXTURE_ADDRESS_CLAMP;
		case AddressMode_Mirror:
			return D3D11_TEXTURE_ADDRESS_MIRROR;
		case AddressMode_Wrap:
			return D3D11_TEXTURE_ADDRESS_WRAP;
		}

		TIKI_BREAK( "[graphics] wrong AddressMode.\n" );
		return D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
	}

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

	SamplerState::SamplerState()
		: m_pSamplerState( nullptr )
	{
	}

	SamplerState::~SamplerState()
	{
		TIKI_ASSERT( m_pSamplerState == nullptr );
	}

	bool SamplerState::create( GraphicsSystem& graphicsSystem, const SamplerStateParamters& creationParamter )
	{
		GraphicsStateObject::create( crcT( &creationParamter ) );

		TIKI_DECLARE_STACKANDZERO( D3D11_SAMPLER_DESC, samplerDesc );
		samplerDesc.Filter			= getFilter( creationParamter.magFilter, creationParamter.mipFilter );
		samplerDesc.AddressU		= getD3dAddressMode( creationParamter.addressU );
		samplerDesc.AddressV		= getD3dAddressMode( creationParamter.addressV );
		samplerDesc.AddressW		= getD3dAddressMode( creationParamter.addressW );
		samplerDesc.MipLODBias		= 0.0f;
		samplerDesc.MaxAnisotropy	= creationParamter.maxAnisotropy;
		samplerDesc.ComparisonFunc	= D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0]	= creationParamter.borderColor.r;
		samplerDesc.BorderColor[1]	= creationParamter.borderColor.g;
		samplerDesc.BorderColor[2]	= creationParamter.borderColor.b;
		samplerDesc.BorderColor[3]	= creationParamter.borderColor.a;
		samplerDesc.MinLOD			= 0;
		samplerDesc.MaxLOD			= D3D11_FLOAT32_MAX;

		HRESULT result = getHandles( graphicsSystem )->pDevice->CreateSamplerState( &samplerDesc, &m_pSamplerState );
		if( FAILED( result ) )
		{
			return false;
		}

		return m_pSamplerState != nullptr;
	}

	void SamplerState::dispose()
	{
		TIKI_ASSERT( m_pSamplerState != nullptr );
		m_pSamplerState->Release();
		m_pSamplerState = nullptr;

		GraphicsStateObject::dispose();
	}
}