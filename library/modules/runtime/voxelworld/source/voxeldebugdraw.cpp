#include "tiki/voxelworld/voxeldebugdraw.hpp"

#include "tiki/debugrenderer/debugrenderer.hpp"
#include "tiki/voxelworld/voxeltypes.hpp"

namespace tiki
{
	namespace voxel
	{

	}

	void voxel::drawVoxel( const Voxel* pVoxel )
	{
		for( uint i = 0u; i < pVoxel->children.getCount(); ++i )
		{
			const Voxel* pChild = pVoxel->children[ i ];
			if( pChild != nullptr )
			{
				drawVoxel( pChild );
			}
		}

		//if( pVoxel->isFull )
		{
			debugrenderer::drawLineAxisAlignedBox( pVoxel->boundingBox, TIKI_COLOR_GRAY );
		}
	}
}
