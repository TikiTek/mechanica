#pragma once

#include "tiki/debug_renderer/debug_renderer.hpp"

#if TIKI_ENABLED( TIKI_DEBUG_RENDERER )

#include <box2d/b2_draw.h>

namespace tiki
{
	class Physics2dDebugDraw : public b2Draw
	{
		TIKI_NONCOPYABLE_CLASS( Physics2dDebugDraw );

	public:

						Physics2dDebugDraw();
		virtual			~Physics2dDebugDraw();

		void			create( float simulationToDrawFactor );
		void			dispose();

		virtual void	DrawPolygon( const b2Vec2* vertices, int32 vertexCount, const b2Color& color ) TIKI_OVERRIDE_FINAL;
		virtual void	DrawSolidPolygon( const b2Vec2* vertices, int32 vertexCount, const b2Color& color ) TIKI_OVERRIDE_FINAL;

		virtual void	DrawCircle( const b2Vec2& center, float32 radius, const b2Color& color ) TIKI_OVERRIDE_FINAL;
		virtual void	DrawSolidCircle( const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color ) TIKI_OVERRIDE_FINAL;

		virtual void	DrawSegment( const b2Vec2& p1, const b2Vec2& p2, const b2Color& color ) TIKI_OVERRIDE_FINAL;
		virtual void	DrawTransform( const b2Transform& xf ) TIKI_OVERRIDE_FINAL;
		virtual void	DrawPoint( const b2Vec2& p, float32 size, const b2Color& color ) TIKI_OVERRIDE_FINAL;

	private:

		float			m_simulationToDrawFactor;
	};
}

#endif
