#pragma once

#include "tiki/base/types.hpp"
#include "tiki/graphics/color.hpp"

#include "base.hpp"

namespace tiki
{
	struct ImmediateVertex
	{
		float3	position;
		uint16	u;
		uint16	v;
		Color	color;
	};
}
