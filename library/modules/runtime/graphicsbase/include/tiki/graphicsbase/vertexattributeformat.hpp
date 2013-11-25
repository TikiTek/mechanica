#pragma once
#ifndef TIKI_VERTEXATTRIBUTEFORMAT_HPP__INCLUDED
#define TIKI_VERTEXATTRIBUTEFORMAT_HPP__INCLUDED

#include "tiki/base/assert.hpp"
#include "tiki/base/inline.hpp"

namespace tiki
{
	enum VertexAttributeFormat
	{
		VertexAttributeFormat_Invalid = -1,

		VertexAttributeFormat_x32y32z32w32_float,
		VertexAttributeFormat_x32y32z32_float,
		VertexAttributeFormat_x32y32_float,
		VertexAttributeFormat_x32_float,

		VertexAttributeFormat_x16y16z16w16_float,
		VertexAttributeFormat_x16y16z16w16_snorm,
		VertexAttributeFormat_x16y16z16w16_unorm,
		VertexAttributeFormat_x16y16_float,
		VertexAttributeFormat_x16y16_snorm,
		VertexAttributeFormat_x16y16_unorm,
		VertexAttributeFormat_x16_float,
		VertexAttributeFormat_x16_snorm,
		VertexAttributeFormat_x16_unorm,

		VertexAttributeFormat_x8y8z8w8,
		VertexAttributeFormat_x8y8z8w8_snorm,
		VertexAttributeFormat_x8y8z8w8_unorm,

		VertexAttributeFormat_Count
	};

	TIKI_FORCE_INLINE size_t getVertexAttributeFormatSize( VertexAttributeFormat format )
	{
		switch ( format )
		{
		case VertexAttributeFormat_x32y32z32w32_float:
			return 16u;
		case VertexAttributeFormat_x32y32z32_float:
			return 12u;
		case VertexAttributeFormat_x32y32_float:
			return 8u;
		case VertexAttributeFormat_x32_float:
			return 4u;

		case VertexAttributeFormat_x16y16z16w16_float:
		case VertexAttributeFormat_x16y16z16w16_snorm:
		case VertexAttributeFormat_x16y16z16w16_unorm:
			return 8u;
		case VertexAttributeFormat_x16y16_float:
		case VertexAttributeFormat_x16y16_snorm:
		case VertexAttributeFormat_x16y16_unorm:
			return 4u;
		case VertexAttributeFormat_x16_float:
		case VertexAttributeFormat_x16_snorm:
		case VertexAttributeFormat_x16_unorm:
			return 2u;

		case VertexAttributeFormat_x8y8z8w8:
		case VertexAttributeFormat_x8y8z8w8_snorm:
		case VertexAttributeFormat_x8y8z8w8_unorm:
			return 4u;

		default:
			TIKI_BREAK( "[graphicsbase] vertex attribute format not spported.\n" );
			break;
		}

		return TIKI_SIZE_T_MAX;
	}

	TIKI_FORCE_INLINE size_t getVertexAttributeFormatElementCount( VertexAttributeFormat format )
	{
		switch ( format )
		{
		case VertexAttributeFormat_x32y32z32w32_float:
			return 4u;
		case VertexAttributeFormat_x32y32z32_float:
			return 3u;
		case VertexAttributeFormat_x32y32_float:
			return 2u;
		case VertexAttributeFormat_x32_float:
			return 1u;

		case VertexAttributeFormat_x16y16z16w16_float:
		case VertexAttributeFormat_x16y16z16w16_snorm:
		case VertexAttributeFormat_x16y16z16w16_unorm:
			return 4u;
		case VertexAttributeFormat_x16y16_float:
		case VertexAttributeFormat_x16y16_snorm:
		case VertexAttributeFormat_x16y16_unorm:
			return 2u;
		case VertexAttributeFormat_x16_float:
		case VertexAttributeFormat_x16_snorm:
		case VertexAttributeFormat_x16_unorm:
			return 1u;

		case VertexAttributeFormat_x8y8z8w8:
		case VertexAttributeFormat_x8y8z8w8_snorm:
		case VertexAttributeFormat_x8y8z8w8_unorm:
			return 4u;

		default:
			TIKI_BREAK( "[graphicsbase] vertex attribute format not spported.\n" );
			break;
		}

		return TIKI_SIZE_T_MAX;
	}
}

#endif // TIKI_VERTEXATTRIBUTEFORMAT_HPP__INCLUDED
