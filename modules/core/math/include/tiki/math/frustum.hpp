#pragma once
#ifndef __TIKI_FRUSTUM_HPP_INCLUDED__
#define __TIKI_FRUSTUM_HPP_INCLUDED__

#include "tiki/math/matrix.hpp"
#include "tiki/math/plane.hpp"

namespace tiki
{
	//struct BoundingBox;
	//struct BoundingSphere;

	enum FrustumPlane
	{
		FrustumPlane_Near,
		FrustumPlane_Far,
		FrustumPlane_Left,
		FrustumPlane_Right,
		FrustumPlane_Top,
		FrustumPlane_Bottom,

		FrustumPlane_Count
	};

	enum FrustumCorner
	{		
		FrustumCorner_NearRightBottom,
		FrustumCorner_NearRightTop,
		FrustumCorner_NearLeftTop,
		FrustumCorner_NearLeftBottom,
		FrustumCorner_FarRightBottom,
		FrustumCorner_FarRightTop,
		FrustumCorner_FarLeftTop,
		FrustumCorner_FarLeftBottom,

		FrustumCorner_Count
	};
	
	enum FrustumIntersectionType
	{
		FrustumIntersectionType_Disjoint,
		FrustumIntersectionType_Contains,
		FrustumIntersectionType_Intersects
	};


	class Frustum
	{
	public:

		void					create( const Matrix44& viewProjection );
		void					create( const Vector3 aCorners[ FrustumCorner_Count ] );

		FrustumIntersectionType	testIntersectionPoint( const Vector3& point ) const;
		//FrustumIntersectionType	testIntersectionBoundingBox( const BoundingBox& box ) const;
		//FrustumIntersectionType	testIntersectionBoundingSphere( const BoundingSphere& sphere ) const;
		
		void					getCorner( Vector3& targetVector, FrustumCorner corner ) const;
		void					getCorners( Vector3 aCorners[ FrustumCorner_Count ] ) const;
		const Plane&			getPlane( FrustumPlane plane ) const;

	private:
				
		Plane					m_planes[ FrustumPlane_Count ];

		void					getThreePlanesIntersectionPoint( Vector3& targetVector, const Plane& plane1, const Plane& plane2, const Plane& plane3 ) const;

	};
}


#endif // __TIKI_FRUSTUM_HPP_INCLUDED__
