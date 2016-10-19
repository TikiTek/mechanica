#pragma once
#ifndef TIKI_GAMECLIENT_HPP__INCLUDED
#define TIKI_GAMECLIENT_HPP__INCLUDED

#include "tiki/entitysystem/entitysystem.hpp"
#include "tiki/math/camera.hpp"
#include "tiki/physics2d/physics2dworld.hpp"
#include "tiki/runtimeshared/freecamera.hpp"

#include "tiki/components/lifetimecomponent.hpp"
#include "tiki/components2d/texturecomponent.hpp"
#include "tiki/components2d/transform2dcomponent.hpp"
#include "tiki/mechanica_components/playercomponent.hpp"
#include "tiki/mechanica_components/wigglecomponent.hpp"
#include "tiki/physics2dcomponents/physics2dbodycomponent.hpp"
#include "tiki/physics2dcomponents/physics2dcollidercomponent.hpp"
#include "tiki/physics2dcomponents/physics2djointcomponent.hpp"

#include "entities.hpp"

namespace tiki
{
	class GraphicsContext;
	class GraphicsSystem;
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
		void											render( GraphicsContext& graphicsContext );

		bool											processInputEvent( const InputEvent& inputEvent );

		EntitySystem&									getEntitySystem()	{ return m_entitySystem; }
		Physics2dWorld&									getPhysicsWorld()	{ return m_physicsWorld; }

		Camera&											getCamera() { return m_gameCamera; }
		const Camera&									getCamera() const { return m_gameCamera; }

	private:

		enum 
		{
			MaxTypeCount	= 16u,
			ChunkCount		= 128u,
			ChunkSize		= 4096u
		};

		EntitySystem							m_entitySystem;

		GraphicsSystem*							m_pGraphicsSystem;

		Physics2dWorld							m_physicsWorld;

		Camera									m_gameCamera;
		FreeCamera								m_freeCamera;

		LifeTimeComponent						m_lifeTimeComponent;
		Physics2dBodyComponent					m_physicsBodyComponent;
		Physics2dColliderComponent				m_physicsColliderComponent;
		Physics2dJointComponent					m_physicsJointComponent;
		PlayerComponent							m_playerComponent;
		TextureComponent						m_textureComponent;
		Transform2dComponent					m_transformComponent;
		WiggleComponent							m_wiggleComponent;
	};
}

#endif // TIKI_GAMECLIENT_HPP__INCLUDED
