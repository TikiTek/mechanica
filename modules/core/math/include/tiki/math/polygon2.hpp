#pragma once

#include "tiki/base/memory.hpp"
#include "tiki/container/array_view.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	struct Line2;

	class Polygon2
	{
	public:

		TIKI_FORCE_INLINE Polygon2()
		{
			disposePolygon();
		}

		TIKI_FORCE_INLINE void createPolygon( Vector2* pPoints, uint pointCount, uint pointCapacity )
		{
			m_pPoints		= pPoints;
			m_pointCount	= pointCount;
			m_pointCapacity	= pointCapacity;
		}

		TIKI_FORCE_INLINE void disposePolygon()
		{
			m_pPoints		= nullptr;
			m_pointCount	= 0u;
			m_pointCapacity	= 0u;
		}

		TIKI_FORCE_INLINE uint getPointCount() const
		{
			return m_pointCount;
		}

		TIKI_FORCE_INLINE uint getPointCapacity() const
		{
			return m_pointCapacity;
		}

		TIKI_FORCE_INLINE const Vector2* getPoints() const
		{
			TIKI_ASSERT( m_pPoints != nullptr );
			return m_pPoints;
		}

		TIKI_FORCE_INLINE const Vector2& getPoint( uint index ) const
		{
			TIKI_ASSERT( m_pPoints != nullptr );
			TIKI_ASSERT( index < m_pointCount );
			return m_pPoints[ index ];
		}

		TIKI_FORCE_INLINE void setPoint( uint index, const Vector2& point )
		{
			TIKI_ASSERT( m_pPoints != nullptr );
			TIKI_ASSERT( index < m_pointCount );
			m_pPoints[ index ] = point;
		}

		TIKI_FORCE_INLINE void pushPoint( const Vector2& point )
		{
			TIKI_ASSERT( m_pPoints != nullptr );
			TIKI_ASSERT( m_pointCount < m_pointCapacity );
			m_pPoints[ m_pointCount++ ] = point;
		}

		TIKI_FORCE_INLINE ArrayView< Vector2 > toArrayView() const
		{
			TIKI_ASSERT( m_pPoints != nullptr );
			return ArrayView< Vector2 >( m_pPoints, m_pointCount );
		}

	private:

		Vector2*	m_pPoints;
		uint		m_pointCount;
		uint		m_pointCapacity;
	};

	template< uint TCapacity >
	class FixedSizePolygon2 : public Polygon2
	{
	public:

		TIKI_FORCE_INLINE FixedSizePolygon2()
		{
			createPolygon( m_aPoints, 0u, TCapacity );
		}

		TIKI_FORCE_INLINE void create( const Vector2* pPoints, uint pointCount )
		{
			TIKI_ASSERT( pointCount <= TCapacity );

			for( uint i = 0u; i < pointCount; ++i )
			{
				m_aPoints[ i ] = pPoints[ i ];
			}

			createPolygon( m_aPoints, pointCount, TCapacity );
		}

	private:

		Vector2 m_aPoints[ TCapacity ];
	};

	class DynamicPolygon2 : public Polygon2
	{
		TIKI_NONCOPYABLE_CLASS( DynamicPolygon2 );

	public:

		TIKI_FORCE_INLINE DynamicPolygon2()
		{
		}

		TIKI_FORCE_INLINE ~DynamicPolygon2()
		{
			TIKI_ASSERT( getPoints() == nullptr );
		}

		TIKI_FORCE_INLINE void create( uint pointCount )
		{
			createPolygon( TIKI_NEW_ARRAY( Vector2, pointCount, false ), 0u, pointCount );
		}

		TIKI_FORCE_INLINE void create( const Vector2* pPoints, uint pointCount )
		{
			Vector2* pNewPoints = TIKI_NEW_ARRAY( Vector2, pointCount, false );
			for( uint i = 0u; i < pointCount; ++i )
			{
				pNewPoints[ i ] = pPoints[ i ];
			}

			createPolygon( pNewPoints, pointCount, pointCount );
		}

		TIKI_FORCE_INLINE void dispose()
		{
			TIKI_FREE( (Vector2*)getPoints() );
			disposePolygon();
		}
	};

	namespace polygon
	{
		bool splitPolygon( Polygon2& polygon1, Polygon2& polygon2, const Polygon2& sourcePolygon, const Line2& cutLine );
	}
}
