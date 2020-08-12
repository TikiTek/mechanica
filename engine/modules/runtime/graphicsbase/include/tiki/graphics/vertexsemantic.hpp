#pragma once
#ifndef TIKI_VERTEXSEMANTIC_HPP__INCLUDED
#define TIKI_VERTEXSEMANTIC_HPP__INCLUDED

#include "tiki/graphics/vertexattributeformat.hpp"

namespace tiki
{
	enum VertexSementic : uint8
	{
		VertexSementic_Position,
		VertexSementic_Normal,
		VertexSementic_TangentFlip,
		VertexSementic_Binormal,
		VertexSementic_Color,
		VertexSementic_TexCoord,
		VertexSementic_JointIndex,
		VertexSementic_JointWeight,

		VertexSementic_Count,
		VertexSementic_Invalid = 0xffu
	};

	TIKI_FORCE_INLINE VertexAttributeFormat getDefaultFormatForSemantic( VertexSementic semantic )
	{
		static const VertexAttributeFormat s_vertexSemanticFormatMapping[] =
		{
			VertexAttributeFormat_x32y32z32_float,		// VertexSementic_Position
			VertexAttributeFormat_x32y32z32_float,		// VertexSementic_Normal
			VertexAttributeFormat_x32y32z32w32_float,	// VertexSementic_TangentFlip
			VertexAttributeFormat_x32y32z32_float,		// VertexSementic_Binormal
			VertexAttributeFormat_x8y8z8w8_unorm,		// VertexSementic_Color
			VertexAttributeFormat_x16y16_float,			// VertexSementic_TexCoord
			VertexAttributeFormat_x8y8z8w8,				// VertexSementic_JointIndex
			VertexAttributeFormat_x16y16z16w16_unorm,	// VertexSementic_JointWeight
		};
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_vertexSemanticFormatMapping ) == VertexSementic_Count );

		return s_vertexSemanticFormatMapping[ semantic ];
	}
}

#endif // TIKI_VERTEXSEMANTIC_HPP__INCLUDED
