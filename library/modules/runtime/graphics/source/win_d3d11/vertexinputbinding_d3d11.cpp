
#include "tiki/graphics/vertexinputbinding.hpp"

#include "tiki/container/array.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/types.hpp"
#include "tiki/graphics/shader.hpp"
#include "tiki/graphics/vertexformat.hpp"
#include "tiki/graphics/vertexsemantic.hpp"

#include <d3d11.h>

namespace tiki
{
	static const char* s_semanticNames[] =
	{
		"POSITION",
		"NORMAL",
		"TANGENT",
		"BINORMAL",
		"COLOR",
		"TEXCOORD",
		"BLENDINDICES",
		"BLENDWEIGHT"
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_semanticNames ) == VertexSementic_Count );

	static const DXGI_FORMAT s_d3dFormat[] =
	{
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_R32G32B32_FLOAT,
		DXGI_FORMAT_R32G32_FLOAT,
		DXGI_FORMAT_R32_FLOAT,
		DXGI_FORMAT_R16G16B16A16_FLOAT,
		DXGI_FORMAT_R16G16B16A16_SNORM,
		DXGI_FORMAT_R16G16B16A16_UNORM,
		DXGI_FORMAT_R16G16_FLOAT,
		DXGI_FORMAT_R16G16_SNORM,
		DXGI_FORMAT_R16G16_UNORM,
		DXGI_FORMAT_R16_FLOAT,
		DXGI_FORMAT_R16_SNORM,
		DXGI_FORMAT_R16_UNORM,
		DXGI_FORMAT_R8G8B8A8_UINT,
		DXGI_FORMAT_R8G8B8A8_SNORM,
		DXGI_FORMAT_R8G8B8A8_UNORM
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_d3dFormat ) == VertexAttributeFormat_Count );

	VertexInputBinding::VertexInputBinding()
	{
		m_pShader					= nullptr;
		m_pVertexFormat				= nullptr;
		m_platformData.pInputLayout	= nullptr;
	}

	VertexInputBinding::~VertexInputBinding()
	{
		m_pShader					= nullptr;
		m_pVertexFormat				= nullptr;
		m_platformData.pInputLayout	= nullptr;
	}

	bool VertexInputBinding::create( GraphicsSystem& graphicsSystem, const VertexInputBindingParameters& creationParameters )
	{
		TIKI_ASSERT( creationParameters.pShader != nullptr );
		TIKI_ASSERT( creationParameters.pShader->getShaderType() == ShaderType_VertexShader );
		TIKI_ASSERT( creationParameters.pVertexFormat != nullptr );

		m_pVertexFormat		= creationParameters.pVertexFormat;
		m_pShader			= creationParameters.pShader;

		D3D11_INPUT_ELEMENT_DESC desc[ GraphicsSystemLimits_MaxVertexAttributes ];

		for (size_t i = 0u; i < m_pVertexFormat->getAttributeCount(); ++i)
		{
			const VertexAttribute& att = m_pVertexFormat->getAttributeByIndex( i );

			desc[ i ].SemanticName			= s_semanticNames[ att.semantic ];
			desc[ i ].SemanticIndex			= att.semanticIndex;
			desc[ i ].Format				= s_d3dFormat[ att.format ];
			desc[ i ].InputSlot				= att.streamIndex;
			desc[ i ].AlignedByteOffset		= D3D11_APPEND_ALIGNED_ELEMENT;
			desc[ i ].InputSlotClass		= ( att.inputType == VertexInputType_PerVertex ? D3D11_INPUT_PER_VERTEX_DATA : D3D11_INPUT_PER_INSTANCE_DATA );
			desc[ i ].InstanceDataStepRate	= 0;
		}
		
		m_platformData.pInputLayout = graphics::createVertexInputLayout( graphicsSystem, m_pShader->m_platformData, desc, m_pVertexFormat->getAttributeCount() );
		if ( m_platformData.pInputLayout == nullptr )
		{
			dispose( graphicsSystem );
			return false;
		}

		return true;
	}

	void VertexInputBinding::dispose( GraphicsSystem& /*graphicsSystem*/ )
	{
		m_pVertexFormat	= nullptr;
		m_pShader		= nullptr;

		if ( m_platformData.pInputLayout != nullptr )
		{
			m_platformData.pInputLayout->Release();
			m_platformData.pInputLayout = nullptr;
		}

		GraphicsStateObject::dispose();
	}
}