#pragma once
#ifndef __TIKI_VIEWPORT_HPP_INCLUDED__
#define __TIKI_VIEWPORT_HPP_INCLUDED__

namespace tiki
{
	struct Viewport
	{
		float	x;
		float	y;
		float	width;
		float	height;
		float	minDepth;
		float	maxDepth;
	};
}

#endif // __TIKI_VIEWPORT_HPP_INCLUDED__
