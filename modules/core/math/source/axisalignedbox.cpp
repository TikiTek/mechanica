#include "tiki/math/axisalignedbox.hpp"

namespace tiki
{
	AxisAlignedBox::AxisAlignedBox()
	{
	}

	void AxisAlignedBox::createFromCenterExtends( const Vector3& _center, const Vector3& _extents )
	{
		center	= _center;
		extents	= _extents;
	}

	void AxisAlignedBox::createFromMinMax( const Vector3& min, const Vector3& max )
	{
		// center = (min + max) / 2
		center = min;
		vector::add( center, max );
		vector::scale( center, 0.5f );
		
		//Extents = (max - min) / 2
		extents = max;
		vector::sub( extents, min );
		vector::scale( extents, 0.5f );
	}

	void AxisAlignedBox::createFromMinMax( float minX, float minY, float minZ, float maxX, float maxY, float maxZ )
	{
		Vector3 min;
		vector::set( min,  minX, minY, minZ );

		Vector3 max;
		vector::set( max, maxX, maxY, maxZ );

		createFromMinMax( min, max);
	}

	void AxisAlignedBox::getVertices( Vector3 aVertices[ AxisAlignedBoxVertices_Count ] ) const
	{
		// Indices Order
		//		   7-------6
		//		  /|      /|
		//		 / |     / |
		//		3--+----2  |
		//      |  4----+--5
		//		| /	    | /
		//		|/      |/
		//		0-------1

		// get extents axis vectors
		//Vector3 extAxis0 = Axis[ 0 ];
		//vector::scale( extAxis0, Extents.x );

		//Vector3 extAxis1 = Axis[ 1 ];
		//vector::scale( extAxis1, Extents.y );

		//Vector3 extAxis2 = Axis[ 2 ];
		//vector::scale( extAxis2, Extents.z );

		//// 0
		//pVertices[ 0 ] = Center;
		//vector::sub( pVertices[ 0 ], extAxis0 );
		//vector::sub( pVertices[ 0 ], extAxis1 );
		//vector::sub( pVertices[ 0 ], extAxis2 );

		//// 1
		//pVertices[ 1 ] = Center;
		//vector::add( pVertices[ 1 ], extAxis0 );
		//vector::sub( pVertices[ 1 ], extAxis1 );
		//vector::sub( pVertices[ 1 ], extAxis2 );

		//// 2
		//pVertices[ 2 ] = Center;
		//vector::add( pVertices[ 2 ], extAxis0 );
		//vector::add( pVertices[ 2 ], extAxis1 );
		//vector::sub( pVertices[ 2 ], extAxis2 );

		//// 3
		//pVertices[ 3 ] = Center;
		//vector::sub( pVertices[ 3 ], extAxis0 );
		//vector::add( pVertices[ 3 ], extAxis1 );
		//vector::sub( pVertices[ 3 ], extAxis2 );

		//// 4
		//pVertices[ 4 ] = Center;
		//vector::sub( pVertices[ 4 ], extAxis0 );
		//vector::sub( pVertices[ 4 ], extAxis1 );
		//vector::add( pVertices[ 4 ], extAxis2 );

		//// 5
		//pVertices[ 5 ] = Center;
		//vector::add( pVertices[ 5 ], extAxis0 );
		//vector::sub( pVertices[ 5 ], extAxis1 );
		//vector::add( pVertices[ 5 ], extAxis2 );

		//// 6
		//pVertices[ 6 ] = Center;
		//vector::add( pVertices[ 6 ], extAxis0 );
		//vector::add( pVertices[ 6 ], extAxis1 );
		//vector::add( pVertices[ 6 ], extAxis2 );

		//// 7
		//pVertices[ 7 ] = Center;
		//vector::sub( pVertices[ 7 ], extAxis0 );
		//vector::add( pVertices[ 7 ], extAxis1 );
		//vector::add( pVertices[ 7 ], extAxis2 );
	}
	
	void AxisAlignedBox::translate( const Vector3& translation )
	{
		vector::add( center, translation );
	}

	void AxisAlignedBox::extend( const Vector3& _extents )
	{
		vector::add( extents, _extents );
	}
}
