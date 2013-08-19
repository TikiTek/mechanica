#pragma once
#ifndef TIKI_VERTEXATTRIBUTE_HPP__INCLUDED
#define TIKI_VERTEXATTRIBUTE_HPP__INCLUDED

#include "tiki/graphicsbase/vertexattributeformat.hpp"
#include "tiki/graphicsbase/vertexsemantic.hpp"

namespace tiki
{
	enum VertexInputType
	{
		VertexInputType_PerVertex,
		VertexInputType_PerInstance
	};

	struct VertexAttribute
	{
		VertexSementic			semantic;
		size_t					semanticIndex;

		VertexAttributeFormat	format;

		size_t					streamIndex;
		VertexInputType			inputType;
	};
}

#endif // TIKI_VERTEXATTRIBUTE_HPP__INCLUDED
