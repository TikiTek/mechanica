
#include "tiki/math/frustum.hpp"

namespace tiki
{
	void Frustum::create( const Matrix44& viewProjection )
	{
		m_planes[ FrustumPlane_Near ].create( viewProjection.x.z, viewProjection.y.z, viewProjection.z.z, viewProjection.w.z );
		m_planes[ FrustumPlane_Far ].create( viewProjection.x.w - viewProjection.x.z, viewProjection.y.w - viewProjection.y.z, viewProjection.z.w - viewProjection.z.z, viewProjection.w.w - viewProjection.w.z );
		m_planes[ FrustumPlane_Left ].create( viewProjection.x.w + viewProjection.x.x, viewProjection.y.w + viewProjection.y.x, viewProjection.z.w + viewProjection.z.x, viewProjection.w.w + viewProjection.w.x );
		m_planes[ FrustumPlane_Right ].create( viewProjection.x.w - viewProjection.x.x, viewProjection.y.w - viewProjection.y.x, viewProjection.z.w - viewProjection.z.x, viewProjection.w.w - viewProjection.w.x );
		m_planes[ FrustumPlane_Top ].create( viewProjection.x.w - viewProjection.x.y, viewProjection.y.w - viewProjection.y.y, viewProjection.z.w - viewProjection.z.y, viewProjection.w.w - viewProjection.w.y );
		m_planes[ FrustumPlane_Bottom ].create( viewProjection.x.w + viewProjection.x.y, viewProjection.y.w + viewProjection.y.y, viewProjection.z.w + viewProjection.z.y, viewProjection.w.w + viewProjection.w.y );		
	}

	FrustumIntersectionType Frustum::testIntersectionPoint( const Vector3& point ) const
	{
		return FrustumIntersectionType_Disjoint;
	}

	//FrustumIntersectionType Frustum::testIntersectionBoundingBox( const BoundingBox& box ) const
	//{
	//	return FrustumIntersectionType_Disjoint;
	//}

	//FrustumIntersectionType Frustum::testIntersectionBoundingSphere( const BoundingSphere& sphere ) const
	//{
	//	return FrustumIntersectionType_Disjoint;
	//}

	void Frustum::getCorner( Vector3& targetVector, FrustumCorner corner ) const
	{
		TIKI_ASSERT( corner < FrustumCorner_Count );

		static const FrustumPlane s_aaCornerPlanes[ FrustumCorner_Count ][ 3u ] =
		{
			{ FrustumPlane_Near,	FrustumPlane_Bottom,	FrustumPlane_Right },
			{ FrustumPlane_Near,	FrustumPlane_Top,		FrustumPlane_Right },
			{ FrustumPlane_Near,	FrustumPlane_Top,		FrustumPlane_Left },
			{ FrustumPlane_Near,	FrustumPlane_Bottom,	FrustumPlane_Left },
			{ FrustumPlane_Far,		FrustumPlane_Bottom,	FrustumPlane_Right },
			{ FrustumPlane_Far,		FrustumPlane_Top,		FrustumPlane_Right },
			{ FrustumPlane_Far,		FrustumPlane_Top,		FrustumPlane_Left },
			{ FrustumPlane_Far,		FrustumPlane_Bottom,	FrustumPlane_Left }
		};

		getThreePlanesIntersectionPoint( targetVector, m_planes[ s_aaCornerPlanes[ corner ][ 0u ] ], m_planes[ s_aaCornerPlanes[ corner ][ 1u ] ], m_planes[ s_aaCornerPlanes[ corner ][ 2u ] ] );
	}

	void Frustum::getCorners( Vector3 aCorners[ FrustumCorner_Count ] ) const
	{
		getThreePlanesIntersectionPoint( aCorners[ FrustumCorner_NearRightBottom ],	m_planes[ FrustumPlane_Near ],	m_planes[ FrustumPlane_Bottom ],	m_planes[ FrustumPlane_Right ] );
		getThreePlanesIntersectionPoint( aCorners[ FrustumCorner_NearRightTop ],	m_planes[ FrustumPlane_Near ],	m_planes[ FrustumPlane_Top ],		m_planes[ FrustumPlane_Right ] );
		getThreePlanesIntersectionPoint( aCorners[ FrustumCorner_NearLeftTop ],		m_planes[ FrustumPlane_Near ],	m_planes[ FrustumPlane_Top ],		m_planes[ FrustumPlane_Left ] );
		getThreePlanesIntersectionPoint( aCorners[ FrustumCorner_NearLeftBottom ],	m_planes[ FrustumPlane_Near ],	m_planes[ FrustumPlane_Bottom ],	m_planes[ FrustumPlane_Left ] );
		getThreePlanesIntersectionPoint( aCorners[ FrustumCorner_FarRightBottom ],	m_planes[ FrustumPlane_Far ],	m_planes[ FrustumPlane_Bottom ],	m_planes[ FrustumPlane_Right ] );
		getThreePlanesIntersectionPoint( aCorners[ FrustumCorner_FarRightTop ],		m_planes[ FrustumPlane_Far ],	m_planes[ FrustumPlane_Top ],		m_planes[ FrustumPlane_Right ] );
		getThreePlanesIntersectionPoint( aCorners[ FrustumCorner_FarLeftTop ],		m_planes[ FrustumPlane_Far ],	m_planes[ FrustumPlane_Top ],		m_planes[ FrustumPlane_Left ] );
		getThreePlanesIntersectionPoint( aCorners[ FrustumCorner_FarLeftBottom ],	m_planes[ FrustumPlane_Far ],	m_planes[ FrustumPlane_Bottom ],	m_planes[ FrustumPlane_Left ] );
	}

	const Plane& Frustum::getPlane( FrustumPlane plane ) const
	{
		return m_planes[ plane ];
	}

	void Frustum::getThreePlanesIntersectionPoint( Vector3& targetVector, const Plane& plane1, const Plane& plane2, const Plane& plane3 ) const
	{
		//P = -d1 * N2xN3 / N1.N2xN3 - d2 * N3xN1 / N2.N3xN1 - d3 * N1xN2 / N3.N1xN2 
		Vector3 normal1;
		Vector3 normal2;
		Vector3 normal3;
		plane1.getNormal( normal1 );
		plane1.getNormal( normal2 );
		plane1.getNormal( normal3 );

		Vector3 cross1;
		Vector3 cross2;
		Vector3 cross3;
		vector::cross( cross1, normal2, normal3 );
		vector::cross( cross2, normal3, normal1 );
		vector::cross( cross3, normal1, normal2 );
		const float dot1 = vector::dot( normal1, cross1 );
		const float dot2 = vector::dot( normal2, cross2 );
		const float dot3 = vector::dot( normal3, cross3 );

		vector::scale( cross1, -plane1.getD() );
		vector::scale( cross2, plane2.getD() );
		vector::scale( cross3, plane3.getD() );
		vector::scale( cross1, 1.0f / dot1 );
		vector::scale( cross2, 1.0f / dot2 );
		vector::scale( cross3, 1.0f / dot3 );
		
		targetVector = cross1;
		vector::sub( targetVector, cross2 );
		vector::sub( targetVector, cross3 );
	}
}