#pragma once
#ifndef __TIKI_PHYSICSDEBUGDRAW_HPP_INCLUDED__
#define __TIKI_PHYSICSDEBUGDRAW_HPP_INCLUDED__

#include "LinearMath/btIDebugDraw.h"

#include "tiki/base/types.hpp"

#if TIKI_DISABLED( TIKI_BUILD_MASTER )

namespace tiki
{
	class PhysicsDebugDraw : public btIDebugDraw
	{
		TIKI_NONCOPYABLE_CLASS( PhysicsDebugDraw );

	public:

						PhysicsDebugDraw();
						~PhysicsDebugDraw();

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

#endif // __TIKI_PHYSICSDEBUGDRAW_HPP_INCLUDED__
