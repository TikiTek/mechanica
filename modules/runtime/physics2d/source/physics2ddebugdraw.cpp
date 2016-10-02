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
	}

	void Physics2dDebugDraw::dispose()
	{
	}

	void Physics2dDebugDraw::DrawPolygon( const b2Vec2* vertices, int32 vertexCount, const b2Color& color )
	{

	}

	void Physics2dDebugDraw::DrawSolidPolygon( const b2Vec2* vertices, int32 vertexCount, const b2Color& color )
	{

	}

	void Physics2dDebugDraw::DrawCircle( const b2Vec2& center, float32 radius, const b2Color& color )
	{

	}

	void Physics2dDebugDraw::DrawSolidCircle( const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color )
	{

	}

	void Physics2dDebugDraw::DrawSegment( const b2Vec2& p1, const b2Vec2& p2, const b2Color& color )
	{

	}

	void Physics2dDebugDraw::DrawTransform( const b2Transform& xf )
	{

	}

	void Physics2dDebugDraw::DrawPoint( const b2Vec2& p, float32 size, const b2Color& color )
	{

	}
#endif
}