#pragma once
#ifndef TIKI_GAMECLIENT_HPP__INCLUDED
#define TIKI_GAMECLIENT_HPP__INCLUDED

#include "tiki/entitysystem/entitysystem.hpp"
#include "tiki/math/camera.hpp"
#include "tiki/physics2d/physics2dworld.hpp"
#include "tiki/runtimeshared/freecamera.hpp"

#include "tiki/components/lifetimecomponent.hpp"
#include "tiki/components2d/polygoncomponent.hpp"
#include "tiki/components2d/spritecomponent.hpp"
#include "tiki/components2d/transform2dcomponent.hpp"
#include "tiki/mechanica_components/breakablecomponent.hpp"
#include "tiki/mechanica_components/playercomponent.hpp"
#include "tiki/mechanica_components/wigglecomponent.hpp"
#include "tiki/physics2dcomponents/physics2dbodycomponent.hpp"
#include "tiki/physics2dcomponents/physics2dcollidercomponent.hpp"
#include "tiki/physics2dcomponents/physics2djointcomponent.hpp"

#include "entities.hpp"

namespace tiki
{
	class GraphicsSystem;
	class Renderer2d;
	struct InputEvent;

	struct GameClientUpdateContext
	{
		float	totalGameTime;
		float	timeDelta;
	};

	class GameClient
	{
		TIKI_NONCOPYABLE_CLASS( GameClient );

	public:

														GameClient();
														~GameClient();

		bool											create( GraphicsSystem& graphicsSystem );
		void											dispose();

		EntityId										createPlayerEntity( const Vector2& position );
		EntityId										createEntityFromTemplate( const EntityTemplateData& entityTemplate );
		//EntityId										createBoxEntity( const Model* pModel, const Vector3& position );
		//EntityId										createCoinEntity( const Model* pModel, const Vector3& position );
		//EntityId										createTerrainEntity( const Model* pModel, const Vector3& position );

		void											disposeEntity( EntityId entityId );

		void											update( GameClientUpdateContext& updateContext );
		void											render( Renderer2d& renderer );

		bool											processInputEvent( const InputEvent& inputEvent );

		EntitySystem&									getEntitySystem()					{ return m_entitySystem; }
		Physics2dWorld&									getPhysicsWorld()					{ return m_physicsWorld; }

		const LifeTimeComponent&						getLifeTimeComponent() const		{ return m_lifeTimeComponent; }
		const Transform2dComponent&						getTransformComponent() const		{ return m_transformComponent; }
		const SpriteComponent&							getSpriteComponent() const			{ return m_spriteComponent; }
		const PolygonComponent&							getPolygonComponent() const			{ return m_polygonComponent; }
		const Physics2dBodyComponent&					getPhysicsBodyComponent() const		{ return m_physicsBodyComponent; }
		const Physics2dColliderComponent&				getPhysicsColliderComponent() const	{ return m_physicsColliderComponent; }
		const Physics2dJointComponent&					getPhysicsJointComponent() const	{ return m_physicsJointComponent; }
		const BreakableComponent&						getBreakableComponent() const		{ return m_breakableComponent; }
		const PlayerComponent&							getPlayerComponent() const			{ return m_playerComponent; }
		const WiggleComponent&							getWiggleComponent() const			{ return m_wiggleComponent; }

	private:

		enum 
		{
			MaxTypeCount	= 16u,
			ChunkCount		= 128u,
			ChunkSize		= 4096u
		};

		EntitySystem					m_entitySystem;

		GraphicsSystem*					m_pGraphicsSystem;

		Physics2dWorld					m_physicsWorld;
		
		LifeTimeComponent				m_lifeTimeComponent;

		Transform2dComponent			m_transformComponent;
		SpriteComponent					m_spriteComponent;
		PolygonComponent				m_polygonComponent;

		Physics2dBodyComponent			m_physicsBodyComponent;
		Physics2dColliderComponent		m_physicsColliderComponent;
		Physics2dJointComponent			m_physicsJointComponent;

		BreakableComponent				m_breakableComponent;
		PlayerComponent					m_playerComponent;
		WiggleComponent					m_wiggleComponent;
	};
}

#endif // TIKI_GAMECLIENT_HPP__INCLUDED
