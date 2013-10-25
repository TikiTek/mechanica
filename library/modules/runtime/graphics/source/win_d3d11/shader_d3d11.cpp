
#include "tiki/graphicsresources/shader.hpp"

#include "tiki/framework/framework.hpp"
#include "tiki/graphics/graphicssystem.hpp"

#include "graphicshandles_d3d11.hpp"

#include <dxgi.h>
#include <d3d11.h>

namespace tiki
{
	Shader::Shader()
	{
		m_type = ShaderType_Invalid;

		m_platformData.pShaderObject	= nullptr;
		m_platformData.pDataBlob		= nullptr;
	}

	Shader::~Shader()
	{
		TIKI_ASSERT( m_type = ShaderType_Invalid );
		TIKI_ASSERT( m_platformData.pDataBlob == nullptr );
		TIKI_ASSERT( m_platformData.pShaderObject = nullptr );
	}

	void Shader::create( GraphicsSystem& graphicsSystem, const void* pData, uint dataSize )
	{
		TIKI_ASSERT( pData != nullptr );
		TIKI_ASSERT( dataSize > 0u );

		const uint32* pInfo		= (const uint32*)pData;
		m_type					= (ShaderType)pInfo[ 0u ];
		const uint32 dataLength	= pInfo[ 1u ];
		
		pData = addPtr( pData, sizeof( uint32 ) * 2u );

		TGDevice* pDevice = graphics::getDevice( graphicsSystem );
		switch ( type )
		{
		case ShaderType_ComputeShader:
			TIKI_VERIFY0( pDevice->CreateComputeShader( pData, dataLength, nullptr, &m_platformData.pComputeShader ) );
			break;
		case ShaderType_DomainShader:
			TIKI_VERIFY0( pDevice->CreateDomainShader( pData, dataLength, nullptr, &m_platformData.pDomainShader ) );
			break;
		case ShaderType_GeometrieShader:
			TIKI_VERIFY0( pDevice->CreateGeometryShader( pData, dataLength, nullptr, &m_platformData.pGeometryShader ) );
			break;
		case ShaderType_HullShader:
			TIKI_VERIFY0( pDevice->CreateHullShader( pData, dataLength, nullptr, &m_platformData.pHullShader ) );
			break;
		case ShaderType_PixelShader:
			TIKI_VERIFY0( pDevice->CreatePixelShader( pData, dataLength, nullptr, &m_platformData.pPixelShader ) );
			break;
		case ShaderType_VertexShader:
			TIKI_VERIFY0( pDevice->CreateVertexShader( pData, dataLength, nullptr, &m_platformData.pVertexShader ) );
			break;
		default:
			TIKI_BREAK( "[graphics] ShaderType not supported.\n" );
			break;
		}

		if ( FAILED( D3D10CreateBlob( dataLength, &m_platformData.pDataBlob ) || m_platformData.pDataBlob == nullptr )
		{
			TIKI_TRACE_ERROR( "[graphics] Could not create Shader-Blob.\n" );
		}

		memory::copy( m_platformData.pDataBlob->GetBufferPointer(), pData, dataLength );
	}

	void Shader::dispose()
	{
		m_type = ShaderType_Invalid;

		if ( m_platformData.pShaderObject != nullptr )
		{
			m_platformData.pShaderObject->Release();
			m_platformData.pShaderObject = nullptr;
		}

		if ( m_platformData.pDataBlob != nullptr )
		{
			m_platformData.pDataBlob->Release();
			m_platformData.pDataBlob = nullptr;
		}
	}

	TGInputLayout* Shader::createInputLayout( TGInputElementDesc* desc, const int count ) const
	{
		TGInputLayout* pLayout = nullptr;

		TGDevice* pDevice = getHandles( framework::getGraphicsSystem() )->pDevice;

		const void* pData	= m_pBlob->GetBufferPointer();
		const size_t size	= m_pBlob->GetBufferSize();

		const HRESULT result = pDevice->CreateInputLayout( desc, count, pData, size, &pLayout );
		TIKI_ASSERT( result == S_OK );

		return pLayout;
	}

}