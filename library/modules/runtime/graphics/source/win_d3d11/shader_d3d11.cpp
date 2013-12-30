
#include "tiki/graphics/shader.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/fixedsizedarray.hpp"
#include "tiki/framework/framework.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphicsbase/shadertype.hpp"
#include "tiki/resource/resourcefile.hpp"

#include "graphicssystem_internal_d3d11.hpp"

namespace tiki
{
	Shader::Shader()
	{
		m_type = ShaderType_Invalid;

		m_platformData.pShaderObject	= nullptr;
		m_platformData.pShaderCode		= nullptr;
		m_platformData.shaderCodeLength	= 0u;
	}

	Shader::~Shader()
	{
		TIKI_ASSERT( m_type == ShaderType_Invalid );
		TIKI_ASSERT( m_platformData.pShaderCode == nullptr );
		TIKI_ASSERT( m_platformData.pShaderObject == nullptr );
	}

	bool Shader::create( GraphicsSystem& graphicsSystem, ShaderType type, const void* pInitData, uint dataSize )
	{
		TIKI_ASSERT( pInitData != nullptr );
		TIKI_ASSERT( dataSize > 0u );

		TGDevice* pDevice = graphics::getDevice( graphicsSystem );

		HRESULT result = S_FALSE;
		switch ( m_type )
		{
		case ShaderType_ComputeShader:
			result = pDevice->CreateComputeShader( pInitData, dataSize, nullptr, &m_platformData.pComputeShader );
			break;
		case ShaderType_DomainShader:
			result = pDevice->CreateDomainShader( pInitData, dataSize, nullptr, &m_platformData.pDomainShader );
			break;
		case ShaderType_GeometrieShader:
			result = pDevice->CreateGeometryShader( pInitData, dataSize, nullptr, &m_platformData.pGeometryShader );
			break;
		case ShaderType_HullShader:
			result = pDevice->CreateHullShader( pInitData, dataSize, nullptr, &m_platformData.pHullShader );
			break;
		case ShaderType_PixelShader:
			result = pDevice->CreatePixelShader( pInitData, dataSize, nullptr, &m_platformData.pPixelShader );
			break;
		case ShaderType_VertexShader:
			result = pDevice->CreateVertexShader( pInitData, dataSize, nullptr, &m_platformData.pVertexShader );
			break;
		default:
			TIKI_BREAK( "[graphics] ShaderType not supported.\n" );
			break;
		}

		if ( FAILED( result ) || m_platformData.pShaderObject == nullptr )
		{
			dispose( graphicsSystem );
			return false;
		}

		m_platformData.pShaderCode		= pInitData;
		m_platformData.shaderCodeLength	= dataSize;

		m_type	= type;
		m_hash	= crcBytes( m_platformData.pShaderCode, m_platformData.shaderCodeLength );
		
		return true;
	}

	void Shader::dispose( GraphicsSystem& graphicsSystem )
	{
		m_type = ShaderType_Invalid;

		if ( m_platformData.pShaderObject != nullptr )
		{
			m_platformData.pShaderObject->Release();
			m_platformData.pShaderObject = nullptr;
		}

		m_platformData.pShaderCode		= nullptr;
		m_platformData.shaderCodeLength	= 0u;
	}

	TGInputLayout* graphics::createVertexInputLayout( const ShaderPlatformData& shaderData, const TGInputElementDesc* pElements, uint elementCount )
	{
		TGDevice* pDevice = graphics::getDevice( framework::getGraphicsSystem() );

		TGInputLayout* pLayout = nullptr;
		const HRESULT result = pDevice->CreateInputLayout(
			pElements,
			elementCount,
			shaderData.pShaderCode,
			shaderData.shaderCodeLength,
			&pLayout
		);

		if ( result == S_OK )
		{
			return pLayout;
		}

		return nullptr;
	}
}