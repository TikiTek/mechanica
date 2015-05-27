#pragma once
#ifndef TIKI_BOX_HPP__
#define TIKI_BOX_HPP__

#include "tiki/math/vector.hpp"


namespace tiki
{
	struct Quaternion;

	struct Box
	{
		TIKI_NONCOPYABLE_STRUCT( Box );

	public:
				Box();
				Box( const Vector3& center, const Vector3& extents );

		void    create( const Vector3& min, const Vector3& max);
		void	create( float minX, float minY, float minZ, float maxX, float maxY, float maxZ );

		void	getVertices( Vector3* pVertices ) const;

		void	translate( const Vector3& translation );
		void	rotate( const Quaternion& rotation );

	public:
		Vector3		Center;
		Vector3		Extents;   
		Vector3		Axis[ 3 ];
	};

}

#endif // TIKI_BOX_HPP__
