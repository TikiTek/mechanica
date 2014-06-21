
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
		"TIKI_INPUT_POSITION0",
		"TIKI_NORMAL0",
		"TIKI_TANGENT0",
		"TIKI_BINORMAL0",
		"TIKI_COLOR0",
		"TIKI_TEXCOORD0",
		"TIKI_BLENDINDICES0",
		"TIKI_BLENDWEIGHT0"
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_apSemanticNames ) == VertexSementic_Count );

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
		m_platformData.locationCount	= 0u;
	}

	VertexInputBinding::~VertexInputBinding()
	{
		m_pShader						= nullptr;
		m_pVertexFormat					= nullptr;
		m_platformData.locationCount	= 0u;
	}

	bool VertexInputBinding::create( GraphicsSystem& graphicsSystem, const VertexInputBindingParameters& creationParameters )
	{
		TIKI_ASSERT( creationParameters.pShader != nullptr );
		TIKI_ASSERT( creationParameters.pShader->getShaderType() == ShaderType_VertexShader );
		TIKI_ASSERT( creationParameters.pVertexFormat != nullptr );

		m_pVertexFormat		= creationParameters.pVertexFormat;
		m_pShader			= creationParameters.pShader;

		m_platformData.locationCount = m_pVertexFormat->getAttributeCount();
		for (size_t i = 0u; i < m_pVertexFormat->getAttributeCount(); ++i)
		{
			const VertexAttribute& att = m_pVertexFormat->getAttributeByIndex( i );
			VertexInputBindingPlatformData::AttributeLocation& location = m_platformData.aLocations[ i ];

			copyString( location.aName, sizeof( location.aName ), s_apSemanticNames[ att.semantic ] );
			const uint stringLength = getStringLength( location.aName );
			location.aName[ stringLength - 1u ] = '0' + att.semanticIndex;

			location.index = i;
		}

		return true;
	}

	void VertexInputBinding::dispose( GraphicsSystem& graphicsSystem )
	{
		m_pVertexFormat	= nullptr;
		m_pShader		= nullptr;

		m_platformData.locationCount = 0u;

		GraphicsStateObject::dispose();
	}
}