#pragma once
#ifndef TIKI_VOXELDEBUGDRAW_HPP_INCLUDED
#define TIKI_VOXELDEBUGDRAW_HPP_INCLUDED

#include "tiki/base/types.hpp"

namespace tiki
{
	struct Voxel;

	namespace voxel
	{
		void	drawVoxel( const Voxel* pVoxel );
	}
}

#endif // TIKI_VOXELDEBUGDRAW_HPP_INCLUDED
