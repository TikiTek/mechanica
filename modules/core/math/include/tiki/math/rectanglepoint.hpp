#pragma once
#ifndef TIKI_RECTANGLEPOINT_HPP_INCLUDED
#define TIKI_RECTANGLEPOINT_HPP_INCLUDED

namespace tiki
{
	enum RectanglePoint
	{
		RectanglePoint_XMinYMax,
		RectanglePoint_XMinYMin,
		RectanglePoint_XMaxYMax,
		RectanglePoint_XMaxYMin,

		RectanglePoint_Count
	};
}

#endif // TIKI_RECTANGLEPOINT_HPP_INCLUDED
