#include "tiki/physics3d/physics3ddebugdraw.hpp"

#include "tiki/debugrenderer/debugrenderer.hpp"
#include "tiki/graphics/color.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
	Physics3dDebugDraw::Physics3dDebugDraw()
	{
		m_debugMode = btIDebugDraw::DBG_NoDebug;
	}

	Physics3dDebugDraw::~Physics3dDebugDraw()
	{
	}

	void Physics3dDebugDraw::create()
	{
		m_debugMode = btIDebugDraw::DBG_DrawWireframe;		
	}

	void Physics3dDebugDraw::dispose()
	{
		m_debugMode = btIDebugDraw::DBG_NoDebug;
	}

	void Physics3dDebugDraw::drawLine( const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor )
	{
		drawLine( from, to, fromColor );
	}

	void Physics3dDebugDraw::drawLine( const btVector3& from, const btVector3& to, const btVector3& color )
	{
		const Color color2	= color::fromFloatRGBA( color.x(), color.y(), color.z(), 1.0f );

		const Vector3 points[] =
		{
			{ from.x(), from.y(), from.z() },
			{ to.x(), to.y(), to.z() }
		};

		debugrenderer::drawLines( points, TIKI_COUNT( points ), color2 );
	}

	void Physics3dDebugDraw::drawSphere( const btVector3& p, btScalar radius, const btVector3& color )
	{
		const Vector3 v ={ p.x(), p.y(), p.z() };
		const Color color2 = color::fromFloatRGBA( color.x(), color.y(), color.z(), 1.0f );
		debugrenderer::drawLineSphere( v, radius, color2 );
	}

	void Physics3dDebugDraw::drawTriangle( const btVector3& a, const btVector3& b, const btVector3& c,const btVector3& color, btScalar alpha )
	{
		const Vector3 v1 = { a.x(), a.y(), a.z() };
		const Vector3 v2 = { b.x(), b.y(), b.z() };
		const Vector3 v3 = { c.x(), c.y(), c.z() };
		const Color color2 = color::fromFloatRGBA( color.x(), color.y(), color.z(), 1.0f );

		const Vector3 points[] = { v1, v2, v3, v1 };
		debugrenderer::drawLines( points, TIKI_COUNT( points ), color2 );
	}

	void Physics3dDebugDraw::drawContactPoint( const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color )
	{
		const btVector3 from	= PointOnB;
		const btVector3 to		= PointOnB + normalOnB;
		
		const Color color2	= color::fromFloatRGBA( color.x(), color.y(), color.z(), 1.0f );

		const Vector3 points[] =
		{
			{ from.x(), from.y(), from.z() },
			{ to.x(), to.y(), to.z() }
		};

		debugrenderer::drawLines( points, TIKI_COUNT( points ), color2 );
	}

	void Physics3dDebugDraw::reportErrorWarning( const char* warningString )
	{
		TIKI_TRACE_WARNING( warningString );
	}

	void Physics3dDebugDraw::draw3dText( const btVector3& /*location*/, const char* /*textString*/ )
	{
	}
#endif
}