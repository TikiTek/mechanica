#include "tiki/physics2d/physics2ddebugdraw.hpp"

#include "tiki/debugrenderer/debugrenderer.hpp"
#include "tiki/graphics/color.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
	Physics2dDebugDraw::Physics2dDebugDraw()
	{
	}

	Physics2dDebugDraw::~Physics2dDebugDraw()
	{
	}

	void Physics2dDebugDraw::create( float simulationToDrawFactor )
	{
		m_simulationToDrawFactor = simulationToDrawFactor;

		m_drawFlags |= e_shapeBit;
		m_drawFlags |= e_jointBit;
		//m_drawFlags |= e_aabbBit;
		m_drawFlags |= e_pairBit;
		m_drawFlags |= e_centerOfMassBit;
	}

	void Physics2dDebugDraw::dispose()
	{
	}

	void Physics2dDebugDraw::DrawPolygon( const b2Vec2* vertices, int32 vertexCount, const b2Color& color )
	{
		TIKI_ASSERT( vertexCount >= 2 );
		const Color color2 = color::fromFloatRGBA( color.r, color.g, color.b, color.a );

		const Vector3 firstPoint = vector::scale( toEngineVector( vertices[ 0u ], 0.0f ), m_simulationToDrawFactor );
		Vector3 lastPoint = firstPoint;
		for( int i = 1u; i < vertexCount; ++i )
		{
			const Vector3 currentPoint = vector::scale( toEngineVector( vertices[ i ], 0.0f ), m_simulationToDrawFactor );
			
			debugrenderer::drawLine( lastPoint, currentPoint, color2 );
			lastPoint = currentPoint;
		}

		debugrenderer::drawLine( lastPoint, firstPoint, color2 );
	}

	void Physics2dDebugDraw::DrawSolidPolygon( const b2Vec2* vertices, int32 vertexCount, const b2Color& color )
	{
		TIKI_ASSERT( vertexCount >= 2 );
		const Color color2 = color::fromFloatRGBA( color.r, color.g, color.b, color.a );

		Vector3 center = Vector3::zero;
		for( uint i = 0u; i < vertexCount; ++i )
		{
			vector::add( center, toEngineVector( vertices[ i ], 0.0f ) );
		}
		vector::scale( center, 1.0f / vertexCount );
		vector::scale( center, m_simulationToDrawFactor );
		
		const Vector3 firstPoint = vector::scale( toEngineVector( vertices[ 0u ], 0.0f ), m_simulationToDrawFactor);
		Vector3 lastPoint = firstPoint;
		for( int i = 1u; i < vertexCount; ++i )
		{
			const Vector3 currentPoint = vector::scale( toEngineVector( vertices[ i ], 0.0f ), m_simulationToDrawFactor );

			debugrenderer::drawSolidTriangle( center, lastPoint, currentPoint, color2 );
			lastPoint = currentPoint;
		}

		debugrenderer::drawSolidTriangle( center, lastPoint, firstPoint, color2 );
	}

	void Physics2dDebugDraw::DrawCircle( const b2Vec2& center, float32 radius, const b2Color& color )
	{
		const Color color2 = color::fromFloatRGBA( color.r, color.g, color.b, color.a );
		debugrenderer::drawLineCircle(
			vector::scale( toEngineVector( center, 0.0f ), m_simulationToDrawFactor ),
			radius * m_simulationToDrawFactor,
			Vector3::unitZ,
			Vector3::unitX,
			color2
		);
	}

	void Physics2dDebugDraw::DrawSolidCircle( const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color )
	{
		const Color color2 = color::fromFloatRGBA( color.r, color.g, color.b, color.a );
		debugrenderer::drawSolidCircle(
			vector::scale( toEngineVector( center, 0.0f ), m_simulationToDrawFactor ), 
			radius * m_simulationToDrawFactor,
			Vector3::unitX,
			Vector3::unitY,
			color2
		);
	}

	void Physics2dDebugDraw::DrawSegment( const b2Vec2& p1, const b2Vec2& p2, const b2Color& color )
	{
		const Color color2 = color::fromFloatRGBA( color.r, color.g, color.b, color.a );
		debugrenderer::drawLine(
			vector::scale( toEngineVector( p1, 0.0f ), m_simulationToDrawFactor ),
			vector::scale( toEngineVector( p2, 0.0f ), m_simulationToDrawFactor ),
			color2 
		);
	}

	void Physics2dDebugDraw::DrawTransform( const b2Transform& xf )
	{

	}

	void Physics2dDebugDraw::DrawPoint( const b2Vec2& p, float32 size, const b2Color& color )
	{
		const Color color2 = color::fromFloatRGBA( color.r, color.g, color.b, color.a );
		debugrenderer::drawSolidRectangle(
			vector::scale( toEngineVector( p, 0.0f ), m_simulationToDrawFactor ),
			vector::create( size, size ),
			Vector3::unitY,
			Vector3::unitX,
			color2 
		);
	}
#endif
}