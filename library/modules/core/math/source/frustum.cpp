
#include "tiki/math/frustum.hpp"

namespace tiki
{
	void Frustum::create( const Matrix44& viewProjection )
	{
		Matrix44 inverseViewProjection;
		matrix::invert( inverseViewProjection, viewProjection );

		const float x0 = -1.0f;
		const float x1 = 1.0f;
		const float y0 = -1.0f;
		const float y1 = 1.0f;
		const float z0 = 0.0f;
		const float z1 = 1.0f;

		Vector3 aCorners[ FrustumCorner_Count ] =
		{
			{ x1, y0, z0 },	// FrustumCorner_NearRightBottom
			{ x1, y1, z0 },	// FrustumCorner_NearRightTop
			{ x0, y1, z0 },	// FrustumCorner_NearLeftTop
			{ x0, y0, z0 },	// FrustumCorner_NearLeftBottom
			{ x1, y0, z1 },	// FrustumCorner_FarRightBottom
			{ x1, y1, z1 },	// FrustumCorner_FarRightTop
			{ x0, y1, z1 },	// FrustumCorner_FarLeftTop
			{ x0, y0, z1 },	// FrustumCorner_FarLeftBottom
		};

		for( uint i = 0u; i < TIKI_COUNT( aCorners); ++i )
		{
			matrix::transform( aCorners[ i ], inverseViewProjection );
		}		

		create( aCorners );
	}

	void Frustum::create( const Vector3 aCorners[ FrustumCorner_Count ] )
	{
		m_planes[ FrustumPlane_Near ].create( aCorners[ FrustumCorner_NearRightTop ], aCorners[ FrustumCorner_NearRightBottom ], aCorners[ FrustumCorner_NearLeftTop ] );
		m_planes[ FrustumPlane_Far ].create( aCorners[ FrustumCorner_FarLeftTop ], aCorners[ FrustumCorner_FarLeftBottom ], aCorners[ FrustumCorner_FarRightTop ] );
		m_planes[ FrustumPlane_Left ].create( aCorners[ FrustumCorner_NearLeftTop ], aCorners[ FrustumCorner_NearLeftBottom ], aCorners[ FrustumCorner_FarLeftTop ] );
		m_planes[ FrustumPlane_Right ].create( aCorners[ FrustumCorner_FarRightTop ], aCorners[ FrustumCorner_FarRightBottom ], aCorners[ FrustumCorner_NearRightTop ] );
		m_planes[ FrustumPlane_Top ].create( aCorners[ FrustumCorner_NearRightBottom ], aCorners[ FrustumCorner_FarRightBottom ], aCorners[ FrustumCorner_NearLeftBottom ] );
		m_planes[ FrustumPlane_Bottom ].create( aCorners[ FrustumCorner_FarRightTop ], aCorners[ FrustumCorner_NearRightTop ], aCorners[ FrustumCorner_FarLeftTop ] );
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

	bool Frustum::getCorner( Vector3& targetVector, FrustumCorner corner ) const
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

		return getThreePlanesIntersectionPoint( targetVector, m_planes[ s_aaCornerPlanes[ corner ][ 0u ] ], m_planes[ s_aaCornerPlanes[ corner ][ 1u ] ], m_planes[ s_aaCornerPlanes[ corner ][ 2u ] ] );
	}

	bool Frustum::getCorners( Vector3 aCorners[ FrustumCorner_Count ] ) const
	{
		bool ok = true;
		//ok &= getThreePlanesIntersectionPoint( aCorners[ FrustumCorner_NearRightBottom ],	m_planes[ FrustumPlane_Near ],	m_planes[ FrustumPlane_Bottom ],	m_planes[ FrustumPlane_Right ] );
		//ok &= getThreePlanesIntersectionPoint( aCorners[ FrustumCorner_NearRightTop ],		m_planes[ FrustumPlane_Near ],	m_planes[ FrustumPlane_Top ],		m_planes[ FrustumPlane_Right ] );
		//ok &= getThreePlanesIntersectionPoint( aCorners[ FrustumCorner_NearLeftTop ],		m_planes[ FrustumPlane_Near ],	m_planes[ FrustumPlane_Top ],		m_planes[ FrustumPlane_Left ] );
		//ok &= getThreePlanesIntersectionPoint( aCorners[ FrustumCorner_NearLeftBottom ],	m_planes[ FrustumPlane_Near ],	m_planes[ FrustumPlane_Bottom ],	m_planes[ FrustumPlane_Left ] );
		//ok &= getThreePlanesIntersectionPoint( aCorners[ FrustumCorner_FarRightBottom ],	m_planes[ FrustumPlane_Far ],	m_planes[ FrustumPlane_Bottom ],	m_planes[ FrustumPlane_Right ] );
		//ok &= getThreePlanesIntersectionPoint( aCorners[ FrustumCorner_FarRightTop ],		m_planes[ FrustumPlane_Far ],	m_planes[ FrustumPlane_Top ],		m_planes[ FrustumPlane_Right ] );
		//ok &= getThreePlanesIntersectionPoint( aCorners[ FrustumCorner_FarLeftTop ],		m_planes[ FrustumPlane_Far ],	m_planes[ FrustumPlane_Top ],		m_planes[ FrustumPlane_Left ] );
		//ok &= getThreePlanesIntersectionPoint( aCorners[ FrustumCorner_FarLeftBottom ],		m_planes[ FrustumPlane_Far ],	m_planes[ FrustumPlane_Bottom ],	m_planes[ FrustumPlane_Left ] );

		ok = ok && getThreePlanesIntersectionPoint( aCorners[ FrustumCorner_NearLeftTop ], m_planes[ FrustumPlane_Left ], m_planes[ FrustumPlane_Near ], m_planes[ FrustumPlane_Top ] );
		ok = ok && getThreePlanesIntersectionPoint( aCorners[ FrustumCorner_NearRightTop ], m_planes[ FrustumPlane_Right ], m_planes[ FrustumPlane_Near ], m_planes[ FrustumPlane_Top ] );
		ok = ok && getThreePlanesIntersectionPoint( aCorners[ FrustumCorner_NearRightBottom ], m_planes[ FrustumPlane_Right ], m_planes[ FrustumPlane_Near ], m_planes[ FrustumPlane_Bottom ] );
		ok = ok && getThreePlanesIntersectionPoint( aCorners[ FrustumCorner_NearLeftBottom ], m_planes[ FrustumPlane_Left ], m_planes[ FrustumPlane_Near ], m_planes[ FrustumPlane_Bottom ] );
		ok = ok && getThreePlanesIntersectionPoint( aCorners[ FrustumCorner_FarLeftTop ], m_planes[ FrustumPlane_Left ], m_planes[ FrustumPlane_Far ], m_planes[ FrustumPlane_Top ] );
		ok = ok && getThreePlanesIntersectionPoint( aCorners[ FrustumCorner_FarRightTop ], m_planes[ FrustumPlane_Right ], m_planes[ FrustumPlane_Far ], m_planes[ FrustumPlane_Top ] );
		ok = ok && getThreePlanesIntersectionPoint( aCorners[ FrustumCorner_FarRightBottom ], m_planes[ FrustumPlane_Right ], m_planes[ FrustumPlane_Far ], m_planes[ FrustumPlane_Bottom ] );
		ok = ok && getThreePlanesIntersectionPoint( aCorners[ FrustumCorner_FarLeftBottom ], m_planes[ FrustumPlane_Left ], m_planes[ FrustumPlane_Far ], m_planes[ FrustumPlane_Bottom ] );

		return ok;
	}

	const Plane& Frustum::getPlane( FrustumPlane plane ) const
	{
		return m_planes[ plane ];
	}

	bool Frustum::getThreePlanesIntersectionPoint( Vector3& targetVector, const Plane& plane1, const Plane& plane2, const Plane& plane3 ) const
	{
		// P = -d1 * N2xN3 / N1.N2xN3 - d2 * N3xN1 / N2.N3xN1 - d3 * N1xN2 / N3.N1xN2 

		const Vector4& data1 = plane1.getData();
		const Vector4& data2 = plane2.getData();
		const Vector4& data3 = plane3.getData();

		Matrix33 mtx;
		mtx.x.x = data1.x;
		mtx.x.y = data2.x;
		mtx.x.z = data3.x;
		mtx.y.x = data1.y;
		mtx.y.y = data2.y;
		mtx.y.z = data3.y;
		mtx.z.x = data1.z;
		mtx.z.y = data2.z;
		mtx.z.z = data3.z;

		Matrix33 inverseMtx;
		if( !matrix::invert( inverseMtx, mtx ) )
		{
			return false;
		}

		vector::set( targetVector, -data1.w, -data2.w, -data3.w );
		matrix::transform( targetVector, inverseMtx );

		return true;
	}
}