#pragma once
#ifndef TIKI_VERTEXSEMANTIC_HPP__INCLUDED
#define TIKI_VERTEXSEMANTIC_HPP__INCLUDED

#include "tiki/graphics/vertexattributeformat.hpp"

namespace tiki
{
	enum VertexSementic
	{
		VertexSementic_Invalid,

		VertexSementic_Position,
		VertexSementic_Normal,
		VertexSementic_TangentFlip,
		VertexSementic_Binormal,
		VertexSementic_Color,
		VertexSementic_TexCoord,
		VertexSementic_JointIndex,
		VertexSementic_JointWeight,

		VertexSementic_Count
	};

	TIKI_FORCE_INLINE VertexAttributeFormat getDefaultFormatForSemantic( VertexSementic semantic )
	{
		switch ( semantic )
		{
		case VertexSementic_Position:
			return VertexAttributeFormat_x32y32z32_float;
		case VertexSementic_Normal:
			return VertexAttributeFormat_x32y32z32_float;
		case VertexSementic_TangentFlip:
			return VertexAttributeFormat_x32y32z32w32_float;
		case VertexSementic_Binormal:
			return VertexAttributeFormat_x32y32z32_float;
		case VertexSementic_Color:
			return VertexAttributeFormat_x8y8z8w8_unorm;
		case VertexSementic_TexCoord:
			return VertexAttributeFormat_x16y16_float;
		case VertexSementic_JointIndex:
			return VertexAttributeFormat_x8y8z8w8;
		case VertexSementic_JointWeight:
			return VertexAttributeFormat_x16y16z16w16_unorm;
			
		default:
			TIKI_BREAK( "[graphics] semantic not supported.\n" );
			break;
		}

		return VertexAttributeFormat_Invalid;
	}
}

#endif // TIKI_VERTEXSEMANTIC_HPP__INCLUDED
