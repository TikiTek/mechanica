
#include "tiki/physics/physicsdebugdraw.hpp"

#include "tiki/graphics/color.hpp"
#include "tiki/graphics/immediaterenderer.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	PhysicsDebugDraw::PhysicsDebugDraw()
	{
		m_debugMode = btIDebugDraw::DBG_NoDebug;

		m_pRenderer = nullptr;
	}

	PhysicsDebugDraw::~PhysicsDebugDraw()
	{
		TIKI_ASSERT( m_pRenderer == nullptr );
	}

	void PhysicsDebugDraw::create()
	{
		m_debugMode = btIDebugDraw::DBG_DrawWireframe;		
	}

	void PhysicsDebugDraw::dispose()
	{
		m_debugMode = btIDebugDraw::DBG_NoDebug;
	}

	void PhysicsDebugDraw::beginRendering( GraphicsContext& graphicsContext, ImmediateRenderer& renderer, const RenderTarget& renderTarget, const Camera& camera )
	{
		m_pRenderer = &renderer;

		m_pRenderer->beginRendering( graphicsContext );
		m_pRenderer->beginRenderPass( &renderTarget, &camera );
	}

	void PhysicsDebugDraw::endRendering()
	{
		m_pRenderer->endRenderPass();
		m_pRenderer->endRendering();

		m_pRenderer = nullptr;
	}

	void PhysicsDebugDraw::drawLine( const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor )
	{
		drawLine( from, to, fromColor );
	}

	void PhysicsDebugDraw::drawLine( const btVector3& from, const btVector3& to, const btVector3& color )
	{
		TIKI_ASSERT( m_pRenderer != nullptr );

		const Color color2	= color::fromFloatRGBA( color.x(), color.y(), color.z(), 1.0f );

		const Vector3 points[] =
		{
			{ from.x(), from.y(), from.z() },
			{ to.x(), to.y(), to.z() }
		};

		m_pRenderer->drawLines( points, TIKI_COUNT( points ), color2 );
	}

	void PhysicsDebugDraw::drawSphere( const btVector3& p, btScalar radius, const btVector3& color )
	{
		TIKI_ASSERT( m_pRenderer != nullptr );

		// todo
	}

	void PhysicsDebugDraw::drawTriangle( const btVector3& a, const btVector3& b, const btVector3& c,const btVector3& color, btScalar alpha )
	{
		TIKI_ASSERT( m_pRenderer != nullptr );

		const Vector3 v1 = { a.x(), a.y(), a.z() };
		const Vector3 v2 = { b.x(), b.y(), b.z() };
		const Vector3 v3 = { c.x(), c.y(), c.z() };
		const Color color2 = color::fromFloatRGBA( color.x(), color.y(), color.z(), 1.0f );

		const Vector3 points[] = { v1, v2, v3, v1 };
		m_pRenderer->drawLines( points, TIKI_COUNT( points ), color2 );
	}

	void PhysicsDebugDraw::drawContactPoint( const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color )
	{
		TIKI_ASSERT( m_pRenderer != nullptr );

		const btVector3 from	= PointOnB;
		const btVector3 to		= PointOnB + normalOnB;
		
		const Color color2	= color::fromFloatRGBA( color.x(), color.y(), color.z(), 1.0f );

		const Vector3 points[] =
		{
			{ from.x(), from.y(), from.z() },
			{ to.x(), to.y(), to.z() }
		};

		m_pRenderer->drawLines( points, TIKI_COUNT( points ), color2 );
	}

	void PhysicsDebugDraw::reportErrorWarning( const char* warningString )
	{
		TIKI_TRACE_WARNING( warningString );
	}

	void PhysicsDebugDraw::draw3dText( const btVector3& /*location*/, const char* /*textString*/ )
	{
	}
}