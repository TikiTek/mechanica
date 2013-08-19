#pragma once
#ifndef TIKI_FONTRECTANGLE_HPP__INCLUDED
#define TIKI_FONTRECTANGLE_HPP__INCLUDED

#include "tiki/base/types.hpp"

namespace tiki
{
	struct FontChar
	{
		float	width;
		float	height;
		uint16	x1;
		uint16	y1;
		uint16	x2;
		uint16	y2;
	};
}

#endif // TIKI_FONTRECTANGLE_HPP__INCLUDED
