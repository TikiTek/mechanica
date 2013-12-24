
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

		m_type	= type;

		TGDevice* pDevice = graphics::getDevice( graphicsSystem );
		switch ( m_type )
		{
		case ShaderType_ComputeShader:
			TIKI_VERIFY0( pDevice->CreateComputeShader( pInitData, dataSize, nullptr, &m_platformData.pComputeShader ) );
			break;
		case ShaderType_DomainShader:
			TIKI_VERIFY0( pDevice->CreateDomainShader( pInitData, dataSize, nullptr, &m_platformData.pDomainShader ) );
			break;
		case ShaderType_GeometrieShader:
			TIKI_VERIFY0( pDevice->CreateGeometryShader( pInitData, dataSize, nullptr, &m_platformData.pGeometryShader ) );
			break;
		case ShaderType_HullShader:
			TIKI_VERIFY0( pDevice->CreateHullShader( pInitData, dataSize, nullptr, &m_platformData.pHullShader ) );
			break;
		case ShaderType_PixelShader:
			TIKI_VERIFY0( pDevice->CreatePixelShader( pInitData, dataSize, nullptr, &m_platformData.pPixelShader ) );
			break;
		case ShaderType_VertexShader:
			TIKI_VERIFY0( pDevice->CreateVertexShader( pInitData, dataSize, nullptr, &m_platformData.pVertexShader ) );
			break;
		default:
			TIKI_BREAK( "[graphics] ShaderType not supported.\n" );
			break;
		}

		m_platformData.pShaderCode		= pInitData;
		m_platformData.shaderCodeLength	= dataSize;
		
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

		m_platformData.pShaderCode		= nullptr;
		m_platformData.shaderCodeLength	= 0u;
	}

	crc32 Shader::getShaderHash() const
	{
		return crcBytes( m_platformData.pShaderCode, m_platformData.shaderCodeLength );
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