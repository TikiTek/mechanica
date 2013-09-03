
#include "tiki/graphics/vertexformat.hpp"

#include "tiki/base/array.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/types.hpp"
#include "tiki/framework/framework.hpp"
#include "tiki/graphicsresources/shader.hpp"

#include "graphicshandles.hpp"
#include <d3d11.h>

namespace tiki
{
	struct VertexFormatInfo
	{
		VertexFormatInfo()
		{
			typeCrc		= 0u;
			refCount	= 0u;
		}

		VertexFormat	format;
		crc32			typeCrc;
		size_t			refCount;
	};

	static Array< VertexFormatInfo > s_formats;

	static cstring s_semanticNames[] =
	{
		"ERROR",
		"POSITION",
		"NORMAL",
		"TANGENT",
		"BINORMAL",
		"COLOR",
		"TEXCOORD",
		"BONEINDICES",
		"BONEWEIGHTS"
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_semanticNames ) == VertexSementic_Count );

	static DXGI_FORMAT getD3DFormat( VertexAttributeFormat format )
	{
		switch ( format )
		{
		case VertexAttributeFormat_x32y32z32w32_float:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case VertexAttributeFormat_x32y32z32_float:
			return DXGI_FORMAT_R32G32B32_FLOAT;
		case VertexAttributeFormat_x32y32_float:
			return DXGI_FORMAT_R32G32_FLOAT;
		case VertexAttributeFormat_x32_float:
			return DXGI_FORMAT_R32_FLOAT;

		case VertexAttributeFormat_x16y16z16w16_float:
			return DXGI_FORMAT_R16G16B16A16_FLOAT;
		case VertexAttributeFormat_x16y16z16w16_snorm:
			return DXGI_FORMAT_R16G16B16A16_SNORM;
		case VertexAttributeFormat_x16y16z16w16_unorm:
			return DXGI_FORMAT_R16G16B16A16_UNORM;
		case VertexAttributeFormat_x16y16_float:
			return DXGI_FORMAT_R16G16_FLOAT;
		case VertexAttributeFormat_x16y16_snorm:
			return DXGI_FORMAT_R16G16_SNORM;
		case VertexAttributeFormat_x16y16_unorm:
			return DXGI_FORMAT_R16G16_UNORM;
		case VertexAttributeFormat_x16_float:
			return DXGI_FORMAT_R16_FLOAT;
		case VertexAttributeFormat_x16_snorm:
			return DXGI_FORMAT_R16_SNORM;
		case VertexAttributeFormat_x16_unorm:
			return DXGI_FORMAT_R16_UNORM;

		case VertexAttributeFormat_x8y8z8w8:
			return DXGI_FORMAT_R8G8B8A8_UINT;
		case VertexAttributeFormat_x8y8z8w8_snorm:
			return DXGI_FORMAT_R8G8B8A8_SNORM;
		case VertexAttributeFormat_x8y8z8w8_unorm:
			return DXGI_FORMAT_R8G8B8A8_UNORM;

		default:
			TIKI_BREAK( "[graphics] format not supported.\n" );
			break;
		}

		return DXGI_FORMAT_UNKNOWN;
	}

	VertexFormat::VertexFormat()
	{
		m_pInputLayout	= nullptr;
		memory::zero( m_vertexStride, sizeof( m_vertexStride ) );
	}

	VertexFormat::~VertexFormat()
	{
		TIKI_ASSERT( m_pInputLayout == nullptr );
	}

	void VertexFormat::initializeSystem()
	{
		s_formats.create( MaxVertexFormatCount );
	}

	void VertexFormat::disposeSystem()
	{
#if TIKI_USE_ASSERT
		size_t count = 0u;
		for (size_t i = 0u; i < s_formats.getCount(); ++i)
		{
			TIKI_ASSERT( s_formats[ i ].typeCrc == 0u );
		}
#endif

		s_formats.dispose();
	}

	const VertexFormat* VertexFormat::getVertexFormat( const VertexFormatParameters& params )
	{
		TIKI_ASSERT( params.pShader != nullptr );

		crc32 crc = crcBytes( &params, sizeof( VertexFormatParameters ) );

		for (size_t i = 0u; i < s_formats.getCount(); ++i)
		{
			VertexFormatInfo& info = s_formats[ i ];

			if ( info.typeCrc == crc )
			{
				info.refCount++;
				return &info.format;
			}
		}

		TGInputElementDesc desc[ MaxFormatDescCount ];

		// TODO: support multi stream vertex stride
		size_t vertexStride[ GraphicsSystemLimits_MaxInputStreams ];
		for (size_t i = 0u; i < params.attributeCount; ++i)
		{
			const VertexAttribute& att = params.pAttributes[ i ];

			desc[ i ].SemanticName			= s_semanticNames[ att.semantic ];
			desc[ i ].SemanticIndex			= att.semanticIndex;
			desc[ i ].Format				= getD3DFormat( att.format );
			desc[ i ].InputSlot				= att.streamIndex;
 			desc[ i ].AlignedByteOffset		= D3D11_APPEND_ALIGNED_ELEMENT;
			desc[ i ].InputSlotClass		= ( att.inputType == VertexInputType_PerVertex ? D3D11_INPUT_PER_VERTEX_DATA : D3D11_INPUT_PER_INSTANCE_DATA );
			desc[ i ].InstanceDataStepRate	= 0;

			vertexStride[ att.streamIndex ] += getVertexAttributeFormatSize( att.format );
		}
		
		uint count = 0;
		for ( uint i = 0; i < s_formats.getCount(); ++i, ++count)
		{
			VertexFormatInfo& info = s_formats[ i ];

			if ( info.typeCrc == 0u )
			{
				TGDevice* pDevice = getHandles( framework::getGraphicsSystem() )->pDevice;

				info.refCount	= 1u;
				info.typeCrc	= crc;
				info.format.m_pInputLayout	= params.pShader->createInputLayout( desc, params.attributeCount );

				for (size_t i = 0u; i < TIKI_COUNT( vertexStride ); ++i)
				{
					info.format.m_vertexStride[ i ] = vertexStride[ i ];
				}

				for (size_t i = 0u; i < params.attributeCount; ++i)
				{
					info.format.m_attributes[ i ] = params.pAttributes[ i ];
				}

				return &info.format;
			}
 		}

		TIKI_ASSERT( count != s_formats.getCount() );
		return nullptr;
	}

	void VertexFormat::releaseVertexFormat( const VertexFormat* pVertexFormat )
	{
		size_t index = ( (size_t)pVertexFormat - (size_t)s_formats.getData() ) / sizeof( VertexFormatInfo );
		TIKI_ASSERT( index < MaxVertexFormatCount );

		VertexFormatInfo& info = s_formats[ index ];
		info.refCount--;

		if ( info.refCount == 0u )
		{
			safeRelease( &info.format.m_pInputLayout );
			info.typeCrc = 0u;
		}
	}
}