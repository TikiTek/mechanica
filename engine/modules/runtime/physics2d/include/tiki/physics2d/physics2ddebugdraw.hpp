#pragma once
#ifndef TIKI_PHYSICS2DDEBUGDRAW_HPP_INCLUDED
#define TIKI_PHYSICS2DDEBUGDRAW_HPP_INCLUDED

#include <Box2D/Common/b2Draw.h>

#include "tiki/base/types.hpp"

#if TIKI_DISABLED( TIKI_BUILD_MASTER )

namespace tiki
{
	class Physics2dDebugDraw : public b2Draw
	{
		TIKI_NONCOPYABLE_CLASS( Physics2dDebugDraw );

	public:

						Physics2dDebugDraw();
		virtual			~Physics2dDebugDraw();

		void			create();
		void			dispose();

		virtual void	DrawPolygon( const b2Vec2* vertices, int32 vertexCount, const b2Color& color ) TIKI_OVERRIDE_FINAL;
		virtual void	DrawSolidPolygon( const b2Vec2* vertices, int32 vertexCount, const b2Color& color ) TIKI_OVERRIDE_FINAL;

		virtual void	DrawCircle( const b2Vec2& center, float32 radius, const b2Color& color ) TIKI_OVERRIDE_FINAL;
		virtual void	DrawSolidCircle( const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color ) TIKI_OVERRIDE_FINAL;

		virtual void	DrawSegment( const b2Vec2& p1, const b2Vec2& p2, const b2Color& color ) TIKI_OVERRIDE_FINAL;
		virtual void	DrawTransform( const b2Transform& xf ) TIKI_OVERRIDE_FINAL;
		virtual void	DrawPoint( const b2Vec2& p, float32 size, const b2Color& color ) TIKI_OVERRIDE_FINAL;
	};
}

#endif

#endif // TIKI_PHYSICS2DDEBUGDRAW_HPP_INCLUDED
