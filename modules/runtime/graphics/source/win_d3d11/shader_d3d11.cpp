
#include "tiki/graphics/shader.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/framework/framework.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphicsbase/shadertype.hpp"

#include "graphicssystem_internal_d3d11.hpp"

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

	bool Shader::create( GraphicsSystem& graphicsSystem, const void* pInitData, uint dataSize )
	{
		TIKI_ASSERT( pInitData != nullptr );
		TIKI_ASSERT( dataSize > 0u );

		const uint32* pInfo		= (const uint32*)pInitData;
		m_type					= (ShaderType)pInfo[ 0u ];
		const uint32 dataLength	= pInfo[ 1u ];
		
		pInitData = addPtr( pInitData, sizeof( uint32 ) * 2u );

		TGDevice* pDevice = graphics::getDevice( graphicsSystem );
		switch ( m_type )
		{
		case ShaderType_ComputeShader:
			TIKI_VERIFY0( pDevice->CreateComputeShader( pInitData, dataLength, nullptr, &m_platformData.pComputeShader ) );
			break;
		case ShaderType_DomainShader:
			TIKI_VERIFY0( pDevice->CreateDomainShader( pInitData, dataLength, nullptr, &m_platformData.pDomainShader ) );
			break;
		case ShaderType_GeometrieShader:
			TIKI_VERIFY0( pDevice->CreateGeometryShader( pInitData, dataLength, nullptr, &m_platformData.pGeometryShader ) );
			break;
		case ShaderType_HullShader:
			TIKI_VERIFY0( pDevice->CreateHullShader( pInitData, dataLength, nullptr, &m_platformData.pHullShader ) );
			break;
		case ShaderType_PixelShader:
			TIKI_VERIFY0( pDevice->CreatePixelShader( pInitData, dataLength, nullptr, &m_platformData.pPixelShader ) );
			break;
		case ShaderType_VertexShader:
			TIKI_VERIFY0( pDevice->CreateVertexShader( pInitData, dataLength, nullptr, &m_platformData.pVertexShader ) );
			break;
		default:
			TIKI_BREAK( "[graphics] ShaderType not supported.\n" );
			break;
		}

		if ( FAILED( D3D10CreateBlob( dataLength, &m_platformData.pDataBlob ) || m_platformData.pDataBlob == nullptr ) )
		{
			TIKI_TRACE_ERROR( "[graphics] Could not create Shader-Blob.\n" );
			return false;
		}

		memory::copy( m_platformData.pDataBlob->GetBufferPointer(), pInitData, dataLength );
		return true;
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

	TGInputLayout* graphics::createVertexInputLayout( const ShaderPlatformData& shaderData, const TGInputElementDesc* pElements, uint elementCount )
	{
		TGInputLayout* pLayout = nullptr;

		TGDevice* pDevice = graphics::getDevice( framework::getGraphicsSystem() );

		const void* pData	= shaderData.pDataBlob->GetBufferPointer();
		const size_t size	= shaderData.pDataBlob->GetBufferSize();

		const HRESULT result = pDevice->CreateInputLayout( pElements, elementCount, pData, size, &pLayout );		
		if ( result == S_OK )
		{
			return pLayout;
		}

		return nullptr;
	}

}