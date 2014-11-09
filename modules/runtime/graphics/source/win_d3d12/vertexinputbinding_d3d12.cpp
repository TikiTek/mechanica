
#include "tiki/graphics/vertexinputbinding.hpp"

#include "tiki/base/array.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/types.hpp"
#include "tiki/graphics/shader.hpp"
#include "tiki/graphics/vertexformat.hpp"
#include "tiki/graphics/vertexsemantic.hpp"

#include "graphicssystem_internal_d3d12.hpp"

namespace tiki
{
	static const char* s_semanticNames[] =
	{
		"ERROR",
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
		m_platformData				= VertexInputBindingPlatformData();
	}

	VertexInputBinding::~VertexInputBinding()
	{
		m_pShader					= nullptr;
		m_pVertexFormat				= nullptr;
		m_platformData				= VertexInputBindingPlatformData();
	}

	bool VertexInputBinding::create( GraphicsSystem& graphicsSystem, const VertexInputBindingParameters& creationParameters )
	{
		TIKI_ASSERT( creationParameters.pShader != nullptr );
		TIKI_ASSERT( creationParameters.pShader->getShaderType() == ShaderType_VertexShader );
		TIKI_ASSERT( creationParameters.pVertexFormat != nullptr );

		m_pVertexFormat		= creationParameters.pVertexFormat;
		m_pShader			= creationParameters.pShader;

		for (size_t i = 0u; i < m_pVertexFormat->getAttributeCount(); ++i)
		{
			const VertexAttribute& att = m_pVertexFormat->getAttributeByIndex( i );
			D3D12_INPUT_ELEMENT_DESC& desc = m_platformData.aInputElements[ i ];

			desc.SemanticName			= s_semanticNames[ att.semantic ];
			desc.SemanticIndex			= att.semanticIndex;
			desc.Format					= s_d3dFormat[ att.format ];
			desc.InputSlot				= att.streamIndex;
			desc.AlignedByteOffset		= D3D12_APPEND_ALIGNED_ELEMENT;
			desc.InputSlotClass			= ( att.inputType == VertexInputType_PerVertex ? D3D12_INPUT_PER_VERTEX_DATA : D3D12_INPUT_PER_INSTANCE_DATA );
			desc.InstanceDataStepRate	= 0;
		}
		
		m_platformData.inputLayoutDesc.NumElements			= m_pVertexFormat->getAttributeCount();
		m_platformData.inputLayoutDesc.pInputElementDescs	= m_platformData.aInputElements;

		return true;
	}

	void VertexInputBinding::dispose( GraphicsSystem& /*graphicsSystem*/ )
	{
		m_pVertexFormat	= nullptr;
		m_pShader		= nullptr;
		m_platformData	= VertexInputBindingPlatformData();

		GraphicsStateObject::dispose();
	}
}