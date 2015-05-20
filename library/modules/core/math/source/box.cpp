
#include "tiki/math/box.hpp"

namespace tiki
{

	Box::Box( const Vector3& center, const Vector3& extents )
		: m_Center( center ), 
		  m_Extents( extents )
	{
		// Init axis aligned
		m_Axis[ 0 ] = Vector3::unitX;
		m_Axis[ 1 ] = Vector3::unitY;
		m_Axis[ 2 ] = Vector3::unitZ;
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
		Vector3 extAxis0 = m_Axis[ 0 ];
		vector::scale( extAxis0, m_Extents.x );

		Vector3 extAxis1 = m_Axis[ 1 ];
		vector::scale( extAxis1, m_Extents.y );

		Vector3 extAxis2 = m_Axis[ 2 ];
		vector::scale( extAxis2, m_Extents.z );

		// 0
		pVertices[ 0 ] = m_Center;
		vector::sub( pVertices[ 0 ], extAxis0 );
		vector::sub( pVertices[ 0 ], extAxis1 );
		vector::sub( pVertices[ 0 ], extAxis2 );

		// 1
		pVertices[ 1 ] = m_Center;
		vector::add( pVertices[ 1 ], extAxis0 );
		vector::sub( pVertices[ 1 ], extAxis1 );
		vector::sub( pVertices[ 1 ], extAxis2 );

		// 2
		pVertices[ 2 ] = m_Center;
		vector::add( pVertices[ 2 ], extAxis0 );
		vector::add( pVertices[ 2 ], extAxis1 );
		vector::sub( pVertices[ 2 ], extAxis2 );

		// 3
		pVertices[ 3 ] = m_Center;
		vector::sub( pVertices[ 3 ], extAxis0 );
		vector::add( pVertices[ 3 ], extAxis1 );
		vector::sub( pVertices[ 3 ], extAxis2 );

		// 4
		pVertices[ 4 ] = m_Center;
		vector::sub( pVertices[ 4 ], extAxis0 );
		vector::sub( pVertices[ 4 ], extAxis1 );
		vector::add( pVertices[ 4 ], extAxis2 );

		// 5
		pVertices[ 5 ] = m_Center;
		vector::add( pVertices[ 5 ], extAxis0 );
		vector::sub( pVertices[ 5 ], extAxis1 );
		vector::add( pVertices[ 5 ], extAxis2 );

		// 6
		pVertices[ 6 ] = m_Center;
		vector::add( pVertices[ 6 ], extAxis0 );
		vector::add( pVertices[ 6 ], extAxis1 );
		vector::add( pVertices[ 6 ], extAxis2 );

		// 7
		pVertices[ 7 ] = m_Center;
		vector::sub( pVertices[ 7 ], extAxis0 );
		vector::add( pVertices[ 7 ], extAxis1 );
		vector::add( pVertices[ 7 ], extAxis2 );
	}

}