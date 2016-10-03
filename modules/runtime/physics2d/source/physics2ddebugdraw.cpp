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

	void Physics2dDebugDraw::create()
	{
		m_drawFlags |= e_shapeBit;
		m_drawFlags |= e_jointBit;
		m_drawFlags |= e_aabbBit;
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

		const Vector3 firstPoint = toTikiVector( vertices[ 0u ], 0.0f );
		Vector3 lastPoint = firstPoint;
		for( int i = 1u; i < vertexCount; ++i )
		{
			const Vector3 currentPoint = toTikiVector( vertices[ i ], 0.0f );
			
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
			vector::add( center, toTikiVector( vertices[ i ], 0.0f ) );
		}
		vector::scale( center, 1.0f / vertexCount );
		
		const Vector3 firstPoint = toTikiVector( vertices[ 0u ], 0.0f );
		Vector3 lastPoint = firstPoint;
		for( int i = 1u; i < vertexCount; ++i )
		{
			const Vector3 currentPoint = toTikiVector( vertices[ i ], 0.0f );

			debugrenderer::drawSolidTriangle( center, lastPoint, currentPoint, color2 );
			lastPoint = currentPoint;
		}

		debugrenderer::drawSolidTriangle( center, lastPoint, firstPoint, color2 );
	}

	void Physics2dDebugDraw::DrawCircle( const b2Vec2& center, float32 radius, const b2Color& color )
	{
		const Color color2 = color::fromFloatRGBA( color.r, color.g, color.b, color.a );
		debugrenderer::drawLineCircle( toTikiVector( center, 0.0f ), radius, Vector3::unitZ, Vector3::unitX, color2 );
	}

	void Physics2dDebugDraw::DrawSolidCircle( const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color )
	{
		const Color color2 = color::fromFloatRGBA( color.r, color.g, color.b, color.a );
		debugrenderer::drawSolidCircle( toTikiVector( center, 0.0f ), radius, Vector3::unitY, Vector3::unitX, color2 );
	}

	void Physics2dDebugDraw::DrawSegment( const b2Vec2& p1, const b2Vec2& p2, const b2Color& color )
	{
		const Color color2 = color::fromFloatRGBA( color.r, color.g, color.b, color.a );
		debugrenderer::drawLine( toTikiVector( p1, 0.0f ), toTikiVector( p2, 0.0f ), color2 );
	}

	void Physics2dDebugDraw::DrawTransform( const b2Transform& xf )
	{

	}

	void Physics2dDebugDraw::DrawPoint( const b2Vec2& p, float32 size, const b2Color& color )
	{
		const Color color2 = color::fromFloatRGBA( color.r, color.g, color.b, color.a );
		debugrenderer::drawSolidRectangle( toTikiVector( p, 0.0f ), vector::create( size, size ), Vector3::unitY, Vector3::unitX, color2 );
	}
#endif
}