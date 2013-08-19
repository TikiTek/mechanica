#include "tiki/graphics/sampler.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/functions.hpp"

#include "tiki/framework/framework.hpp"
#include "tiki/graphics/graphicsystem.hpp"
#include "graphicshandles.hpp"

#include <d3d11.h>

namespace tiki
{


	Sampler::Sampler()
		: m_pSampler( nullptr )
	{

	}

	Sampler::~Sampler()
	{
		TIKI_ASSERT( m_pSampler == nullptr );
	}

	void Sampler::create()
	{
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory( &samplerDesc, sizeof( samplerDesc ) );
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		GraphicsHandles* pHandles = getHandles( framework::getGraphicsSystem() );
		HRESULT result = pHandles->pDevice->CreateSamplerState( &samplerDesc, &m_pSampler );
		if( FAILED( result ) )
		{
			debug::breakPoint();
		}

		TIKI_ASSERT( m_pSampler );
	}

	void Sampler::dispose()
	{
		TIKI_ASSERT( m_pSampler != nullptr );
		m_pSampler->Release();
		m_pSampler = nullptr;
	}

}