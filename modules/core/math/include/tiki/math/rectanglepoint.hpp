#pragma once
#ifndef TIKI_RECTANGLEPOINT_HPP_INCLUDED
#define TIKI_RECTANGLEPOINT_HPP_INCLUDED

namespace tiki
{
	enum RectanglePoint
	{
		RectanglePoint_XMinYMin,
		RectanglePoint_XMaxYMin,
		RectanglePoint_XMaxYMax,
		RectanglePoint_XMinYMax,

		RectanglePoint_Count
	};
}

#endif // TIKI_RECTANGLEPOINT_HPP_INCLUDED
