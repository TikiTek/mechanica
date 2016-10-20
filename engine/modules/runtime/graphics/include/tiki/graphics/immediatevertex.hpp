#pragma once
#ifndef TIKI_IMMEDIATEVERTEX_HPP_INCLUDED
#define TIKI_IMMEDIATEVERTEX_HPP_INCLUDED

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

#endif // TIKI_IMMEDIATEVERTEX_HPP_INCLUDED
