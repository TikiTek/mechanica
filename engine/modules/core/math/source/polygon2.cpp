#include "tiki/math/polygon2.hpp"

namespace tiki
{
	bool polygon::splitPolygon( Polygon2& polygon1, Polygon2& polygon2, const Polygon2& sourcePolygon, const Line2& cutLine )
	{
		TIKI_ASSERT( polygon1.getPointCapacity() >= sourcePolygon.getPointCount() );
		TIKI_ASSERT( polygon2.getPointCapacity() >= sourcePolygon.getPointCount() );

		Line2 intersectionLine;
		intersectionLine.start = sourcePolygon.getPoint( sourcePolygon.getPointCount() - 1u );

		Vector2 lastPoint = intersectionLine.start;

		uint intersectionCount = 0u;
		for( uint i = 1u; i < sourcePolygon.getPointCount(); ++i )
		{
			const Vector2& currentPoint = sourcePolygon.getPoint( i );
			intersectionLine.end = currentPoint;

			Vector2 intersectionPoint;
			if( intersection::checkLineLineIntersection( cutLine, intersectionLine, intersectionPoint ) )
			{
				TIKI_ASSERT( polygon1.getPointCount() == 0u || !vector::isEquals( intersectionPoint, polygon1.getPoint( polygon1.getPointCount() - 1u ), 0.005f ) );
				polygon1.pushPoint( intersectionPoint );

				TIKI_ASSERT( polygon2.getPointCount() == 0u || !vector::isEquals( intersectionPoint, polygon2.getPoint( polygon2.getPointCount() - 1u ), 0.005f ) );
				polygon2.pushPoint( intersectionPoint );

				intersectionCount++;
			}

			if( intersectionCount == 1u )
			{
				TIKI_ASSERT( polygon2.getPointCount() == 0u || !vector::isEquals( currentPoint, polygon2.getPoint( polygon2.getPointCount() - 1u ), 0.005f ) );
				polygon2.pushPoint( currentPoint );
			}
			else
			{
				TIKI_ASSERT( polygon1.getPointCount() == 0u || !vector::isEquals( currentPoint, polygon1.getPoint( polygon1.getPointCount() - 1u ), 0.005f ) );
				polygon1.pushPoint( currentPoint );
			}

			lastPoint = currentPoint;
			intersectionLine.start = intersectionLine.end;
		}

		return intersectionCount == 0u || intersectionCount == 2u;
	}
}
