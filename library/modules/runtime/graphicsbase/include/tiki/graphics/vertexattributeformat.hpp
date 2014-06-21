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

	TIKI_FORCE_INLINE uint getVertexAttributeFormatSize( VertexAttributeFormat format )
	{
		static const uint s_aVertexAttributeFormatSizeMapping[] =
		{
			16u,	// VertexAttributeFormat_x32y32z32w32_float
			12u,	// VertexAttributeFormat_x32y32z32_float
			8u,		// VertexAttributeFormat_x32y32_float
			4u,		// VertexAttributeFormat_x32_float
			8u,		// VertexAttributeFormat_x16y16z16w16_float
			8u,		// VertexAttributeFormat_x16y16z16w16_snorm
			8u,		// VertexAttributeFormat_x16y16z16w16_unorm
			4u,		// VertexAttributeFormat_x16y16_float
			4u,		// VertexAttributeFormat_x16y16_snorm
			4u,		// VertexAttributeFormat_x16y16_unorm
			2u,		// VertexAttributeFormat_x16_float
			2u,		// VertexAttributeFormat_x16_snorm
			2u,		// VertexAttributeFormat_x16_unorm
			4u,		// VertexAttributeFormat_x8y8z8w8
			4u,		// VertexAttributeFormat_x8y8z8w8_snorm
			4u		// VertexAttributeFormat_x8y8z8w8_unorm
		};
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aVertexAttributeFormatSizeMapping ) == VertexAttributeFormat_Count );

		return s_aVertexAttributeFormatSizeMapping[ format ];
	}

	TIKI_FORCE_INLINE uint getVertexAttributeFormatElementCount( VertexAttributeFormat format )
	{
		static const uint s_aVertexAttributeFormatElementCountMapping[] =
		{
			4u,		// VertexAttributeFormat_x32y32z32w32_float
			3u,		// VertexAttributeFormat_x32y32z32_float
			2u,		// VertexAttributeFormat_x32y32_float
			1u,		// VertexAttributeFormat_x32_float
			4u,		// VertexAttributeFormat_x16y16z16w16_float
			4u,		// VertexAttributeFormat_x16y16z16w16_snorm
			4u,		// VertexAttributeFormat_x16y16z16w16_unorm
			2u,		// VertexAttributeFormat_x16y16_float
			2u,		// VertexAttributeFormat_x16y16_snorm
			2u,		// VertexAttributeFormat_x16y16_unorm
			1u,		// VertexAttributeFormat_x16_float
			1u,		// VertexAttributeFormat_x16_snorm
			1u,		// VertexAttributeFormat_x16_unorm
			4u,		// VertexAttributeFormat_x8y8z8w8
			4u,		// VertexAttributeFormat_x8y8z8w8_snorm
			4u		// VertexAttributeFormat_x8y8z8w8_unorm			
		};
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aVertexAttributeFormatElementCountMapping ) == VertexAttributeFormat_Count );

		return s_aVertexAttributeFormatElementCountMapping[ format ];
	}
}

#endif // TIKI_VERTEXATTRIBUTEFORMAT_HPP__INCLUDED
