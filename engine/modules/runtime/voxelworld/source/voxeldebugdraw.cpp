#include "tiki/voxelworld/voxeldebugdraw.hpp"

#include "tiki/base/debugprop.hpp"
#include "tiki/debugrenderer/debugrenderer.hpp"
#include "tiki/voxelworld/voxeltypes.hpp"

namespace tiki
{
	TIKI_DEBUGPROP_INT( s_maxRenderDepth, "voxel/debug/maxRenderDepth", 32u, 1u, 32u );

	namespace voxel
	{
		void drawVoxel( const Voxel* pVoxel, uint depth );
	}

	void voxel::drawVoxel( const Voxel* pVoxel )
	{
		drawVoxel( pVoxel, 0u );
	}

	void voxel::drawVoxel( const Voxel* pVoxel, uint depth )
	{
		if( depth > s_maxRenderDepth )
		{
			return;
		}

		for( uint i = 0u; i < pVoxel->children.getCount(); ++i )
		{
			const Voxel* pChild = pVoxel->children[ i ];
			if( pChild != nullptr )
			{
				drawVoxel( pChild, depth + 1u );
			}
		}

		if( pVoxel->isFull )
		{
			debugrenderer::drawLineAxisAlignedBox( pVoxel->boundingBox, TIKI_COLOR_GRAY );
		}
	}
}
