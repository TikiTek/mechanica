#pragma once
#ifndef TIKI_VERTEXATTRIBUTE_HPP__INCLUDED
#define TIKI_VERTEXATTRIBUTE_HPP__INCLUDED

#include "tiki/graphics/vertexattributeformat.hpp"
#include "tiki/graphics/vertexsemantic.hpp"

namespace tiki
{
	enum VertexInputType : uint8
	{
		VertexInputType_PerVertex,
		VertexInputType_PerInstance
	};

	struct VertexAttribute
	{
		VertexSementic			semantic;
		uint8					semanticIndex;

		VertexAttributeFormat	format;

		uint8					streamIndex;
		VertexInputType			inputType;
	};
}

#endif // TIKI_VERTEXATTRIBUTE_HPP__INCLUDED
