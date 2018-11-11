#pragma once

#include "tiki/graphics/vertex_attribute_format.hpp"
#include "tiki/graphics/vertex_semantic.hpp"

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
