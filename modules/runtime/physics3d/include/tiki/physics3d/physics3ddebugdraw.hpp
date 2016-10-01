#pragma once
#ifndef TIKI_PHYSICS3DDEBUGDRAW_HPP_INCLUDED
#define TIKI_PHYSICS3DDEBUGDRAW_HPP_INCLUDED

#include "LinearMath/btIDebugDraw.h"

#include "tiki/base/types.hpp"

#if TIKI_DISABLED( TIKI_BUILD_MASTER )

namespace tiki
{
	class Physics3dDebugDraw : public btIDebugDraw
	{
		TIKI_NONCOPYABLE_CLASS( Physics3dDebugDraw );

	public:

						Physics3dDebugDraw();
						~Physics3dDebugDraw();

		void			create();
		void			dispose();

		virtual void	drawLine( const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor );
		virtual void	drawLine( const btVector3& from, const btVector3& to, const btVector3& color );
		virtual void	drawTriangle( const btVector3& a, const btVector3& b, const btVector3& c, const btVector3& color, btScalar alpha );

		virtual void	drawSphere( const btVector3& p, btScalar radius, const btVector3& color );
		virtual void	drawContactPoint( const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color );
		virtual void	draw3dText( const btVector3& location, const char* textString );

		virtual void	reportErrorWarning( const char* warningString );

		virtual void	setDebugMode( int debugMode )	{ m_debugMode = debugMode; }
		virtual int		getDebugMode() const			{ return m_debugMode; }

	private:
		
		int				m_debugMode;

	};
}

#endif

#endif // TIKI_PHYSICS3DDEBUGDRAW_HPP_INCLUDED
