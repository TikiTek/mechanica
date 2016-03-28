
#include "tiki/math/box.hpp"
#include "tiki/math/quaternion.hpp"

namespace tiki
{
	Box::Box()
	{
		vector::clear( center );
		vector::clear( extents );

		// Init axis aligned
		axis[ 0u ] = Vector3::unitX;
		axis[ 1u ] = Vector3::unitY;
		axis[ 2u ] = Vector3::unitZ;
	}

	Box::Box( const Vector3& _center, const Vector3& _extents )
	{
		center		= _center;
		extents		= _extents;

		// Init axis aligned
		axis[ 0u ]	= Vector3::unitX;
		axis[ 1u ]	= Vector3::unitY;
		axis[ 2u ]	= Vector3::unitZ;
	}

	void Box::create( const Vector3& min, const Vector3& max )
	{
		// center = (min + max) / 2
		center = min;
		vector::add( center, max );
		vector::scale( center, 0.5f );
		
		// extents = (max - min) / 2
		extents = max;
		vector::sub( extents, min );
		vector::scale( extents, 0.5f );
	}

	void Box::create( float minX, float minY, float minZ, float maxX, float maxY, float maxZ )
	{
		Vector3 min;
		vector::set( min,  minX, minY, minZ );

		Vector3 max;
		vector::set( max, maxX, maxY, maxZ );

		create( min, max);
	}

	void Box::getVertices( Vector3* pVertices ) const
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
		Vector3 extAxis0 = axis[ 0 ];
		vector::scale( extAxis0, extents.x );

		Vector3 extAxis1 = axis[ 1 ];
		vector::scale( extAxis1, extents.y );

		Vector3 extAxis2 = axis[ 2 ];
		vector::scale( extAxis2, extents.z );

		// 0
		pVertices[ 0 ] = center;
		vector::sub( pVertices[ 0 ], extAxis0 );
		vector::sub( pVertices[ 0 ], extAxis1 );
		vector::sub( pVertices[ 0 ], extAxis2 );

		// 1
		pVertices[ 1 ] = center;
		vector::add( pVertices[ 1 ], extAxis0 );
		vector::sub( pVertices[ 1 ], extAxis1 );
		vector::sub( pVertices[ 1 ], extAxis2 );

		// 2
		pVertices[ 2 ] = center;
		vector::add( pVertices[ 2 ], extAxis0 );
		vector::add( pVertices[ 2 ], extAxis1 );
		vector::sub( pVertices[ 2 ], extAxis2 );

		// 3
		pVertices[ 3 ] = center;
		vector::sub( pVertices[ 3 ], extAxis0 );
		vector::add( pVertices[ 3 ], extAxis1 );
		vector::sub( pVertices[ 3 ], extAxis2 );

		// 4
		pVertices[ 4 ] = center;
		vector::sub( pVertices[ 4 ], extAxis0 );
		vector::sub( pVertices[ 4 ], extAxis1 );
		vector::add( pVertices[ 4 ], extAxis2 );

		// 5
		pVertices[ 5 ] = center;
		vector::add( pVertices[ 5 ], extAxis0 );
		vector::sub( pVertices[ 5 ], extAxis1 );
		vector::add( pVertices[ 5 ], extAxis2 );

		// 6
		pVertices[ 6 ] = center;
		vector::add( pVertices[ 6 ], extAxis0 );
		vector::add( pVertices[ 6 ], extAxis1 );
		vector::add( pVertices[ 6 ], extAxis2 );

		// 7
		pVertices[ 7 ] = center;
		vector::sub( pVertices[ 7 ], extAxis0 );
		vector::add( pVertices[ 7 ], extAxis1 );
		vector::add( pVertices[ 7 ], extAxis2 );
	}

	void Box::translate( const Vector3& translation )
	{
		vector::add( center, translation );
	}

	void Box::rotate( const Quaternion& rotation )
	{
		quaternion::transform( center, rotation );
		quaternion::transform( axis[ 0 ], rotation );
		quaternion::transform( axis[ 1 ], rotation );
		quaternion::transform( axis[ 2 ], rotation );
	}
}
