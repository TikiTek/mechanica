
#include "tiki/graphics/vertexinputbinding.hpp"

#include "tiki/base/array.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/types.hpp"
#include "tiki/graphics/shader.hpp"
#include "tiki/graphics/vertexformat.hpp"
#include "tiki/graphics/vertexsemantic.hpp"

#include "graphicssystem_internal_opengl4.hpp"

namespace tiki
{
	static const char* s_apSemanticNames[] =
	{
		"ERROR",
		"inTIKI_INPUT_POSITION0",
		"inTIKI_NORMAL0",
		"inTIKI_TANGENT0",
		"inTIKI_BINORMAL0",
		"inTIKI_COLOR0",
		"inTIKI_TEXCOORD0",
		"inTIKI_BLENDINDICES0",
		"inTIKI_BLENDWEIGHT0"
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_apSemanticNames ) == VertexSementic_Count );

	static const GLenum s_aVertexAttributeFormatTypeMapping[] =
	{
		GL_FLOAT,			// VertexAttributeFormat_x32y32z32w32_float
		GL_FLOAT,			// VertexAttributeFormat_x32y32z32_float
		GL_FLOAT,			// VertexAttributeFormat_x32y32_float
		GL_FLOAT,			// VertexAttributeFormat_x32_float
		GL_HALF_FLOAT,		// VertexAttributeFormat_x16y16z16w16_float
		GL_SHORT,			// VertexAttributeFormat_x16y16z16w16_snorm
		GL_UNSIGNED_SHORT,	// VertexAttributeFormat_x16y16z16w16_unorm
		GL_HALF_FLOAT,		// VertexAttributeFormat_x16y16_float
		GL_SHORT,			// VertexAttributeFormat_x16y16_snorm
		GL_UNSIGNED_SHORT,	// VertexAttributeFormat_x16y16_unorm
		GL_HALF_FLOAT,		// VertexAttributeFormat_x16_float
		GL_SHORT,			// VertexAttributeFormat_x16_snorm
		GL_UNSIGNED_SHORT,	// VertexAttributeFormat_x16_unorm
		GL_BYTE,			// VertexAttributeFormat_x8y8z8w8
		GL_BYTE,			// VertexAttributeFormat_x8y8z8w8_snorm
		GL_UNSIGNED_BYTE	// VertexAttributeFormat_x8y8z8w8_unorm
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aVertexAttributeFormatTypeMapping ) == VertexAttributeFormat_Count );

	static const GLboolean s_aVertexAttributeFormatNormalizedMapping[] =
	{
		GL_FALSE,	// VertexAttributeFormat_x32y32z32w32_float
		GL_FALSE,	// VertexAttributeFormat_x32y32z32_float
		GL_FALSE,	// VertexAttributeFormat_x32y32_float
		GL_FALSE,	// VertexAttributeFormat_x32_float
		GL_FALSE,	// VertexAttributeFormat_x16y16z16w16_float
		GL_TRUE,	// VertexAttributeFormat_x16y16z16w16_snorm
		GL_TRUE,	// VertexAttributeFormat_x16y16z16w16_unorm
		GL_FALSE,	// VertexAttributeFormat_x16y16_float
		GL_TRUE,	// VertexAttributeFormat_x16y16_snorm
		GL_TRUE,	// VertexAttributeFormat_x16y16_unorm
		GL_FALSE,	// VertexAttributeFormat_x16_float
		GL_TRUE,	// VertexAttributeFormat_x16_snorm
		GL_TRUE,	// VertexAttributeFormat_x16_unorm
		GL_FALSE,	// VertexAttributeFormat_x8y8z8w8
		GL_TRUE,	// VertexAttributeFormat_x8y8z8w8_snorm
		GL_TRUE		// VertexAttributeFormat_x8y8z8w8_unorm
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aVertexAttributeFormatNormalizedMapping ) == VertexAttributeFormat_Count );

	//static const DXGI_FORMAT s_d3dFormat[] =
	//{
	//	DXGI_FORMAT_R32G32B32A32_FLOAT,
	//	DXGI_FORMAT_R32G32B32_FLOAT,
	//	DXGI_FORMAT_R32G32_FLOAT,
	//	DXGI_FORMAT_R32_FLOAT,
	//	DXGI_FORMAT_R16G16B16A16_FLOAT,
	//	DXGI_FORMAT_R16G16B16A16_SNORM,
	//	DXGI_FORMAT_R16G16B16A16_UNORM,
	//	DXGI_FORMAT_R16G16_FLOAT,
	//	DXGI_FORMAT_R16G16_SNORM,
	//	DXGI_FORMAT_R16G16_UNORM,
	//	DXGI_FORMAT_R16_FLOAT,
	//	DXGI_FORMAT_R16_SNORM,
	//	DXGI_FORMAT_R16_UNORM,
	//	DXGI_FORMAT_R8G8B8A8_UINT,
	//	DXGI_FORMAT_R8G8B8A8_SNORM,
	//	DXGI_FORMAT_R8G8B8A8_UNORM
	//};
	//TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_d3dFormat ) == VertexAttributeFormat_Count );

	VertexInputBinding::VertexInputBinding()
	{
		m_pShader						= nullptr;
		m_pVertexFormat					= nullptr;
	}

	VertexInputBinding::~VertexInputBinding()
	{
		m_pShader						= nullptr;
		m_pVertexFormat					= nullptr;
	}

	bool VertexInputBinding::create( GraphicsSystem& graphicsSystem, const VertexInputBindingParameters& creationParameters )
	{
		TIKI_ASSERT( creationParameters.pShader != nullptr );
		TIKI_ASSERT( creationParameters.pShader->getShaderType() == ShaderType_VertexShader );
		TIKI_ASSERT( creationParameters.pVertexFormat != nullptr );

		m_pVertexFormat		= creationParameters.pVertexFormat;
		m_pShader			= creationParameters.pShader;

		uint currentOffset = 0u;
		for (size_t i = 0u; i < m_pVertexFormat->getAttributeCount(); ++i)
		{
			const VertexAttribute& att = m_pVertexFormat->getAttributeByIndex( i );
			VertexInputBindingPlatformData::VertexAttribute& attribute = m_platformData.aAttributes[ i ];

			attribute.elementCount		= getVertexAttributeFormatElementCount( att.format );
			attribute.elementType		= s_aVertexAttributeFormatTypeMapping[ att.format ];
			attribute.elementNormalized	= s_aVertexAttributeFormatNormalizedMapping[ att.format ];
			attribute.elementOffset		= currentOffset;

			currentOffset += getVertexAttributeFormatSize( att.format );
		}
		
		m_platformData.attributeCount	= m_pVertexFormat->getAttributeCount();
		m_platformData.vertexStride		= currentOffset;

		return true;
	}

	void VertexInputBinding::dispose( GraphicsSystem& graphicsSystem )
	{
		m_pVertexFormat	= nullptr;
		m_pShader		= nullptr;

		m_platformData.attributeCount	= 0u;
		m_platformData.vertexStride		= 0u;

		GraphicsStateObject::dispose();
	}
}