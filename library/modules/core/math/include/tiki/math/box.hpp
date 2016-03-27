#pragma once
#ifndef TIKI_BOX_HPP__
#define TIKI_BOX_HPP__

#include "tiki/math/vector.hpp"

namespace tiki
{
	struct Quaternion;

	struct Box
	{
	public:

					Box();
					Box( const Vector3& _center, const Vector3& _extents );

		void		create( const Vector3& min, const Vector3& max);
		void		create( float minX, float minY, float minZ, float maxX, float maxY, float maxZ );

		void		getVertices( Vector3* pVertices ) const;

		void		translate( const Vector3& translation );
		void		rotate( const Quaternion& rotation );

		Vector3		center;
		Vector3		extents;   
		Vector3		axis[ 3u ];
	};

}

#endif // TIKI_BOX_HPP__
