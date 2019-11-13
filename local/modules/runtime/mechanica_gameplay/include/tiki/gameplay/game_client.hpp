#pragma once

#include "tiki/base/timer.hpp"
#include "tiki/entity_system/entity_system.hpp"
#include "tiki/math/camera.hpp"
#include "tiki/physics_2d/physics_2d_world.hpp"
#include "tiki/runtime_shared/free_camera.hpp"

#include "tiki/components/life_time_component.hpp"
#include "tiki/components_2d/polygon_component.hpp"
#include "tiki/components_2d/sprite_component.hpp"
#include "tiki/components_2d/transform_2d_component.hpp"
#include "tiki/mechanica_components/breakable_component.hpp"
#include "tiki/mechanica_components/player_component.hpp"
#include "tiki/mechanica_components/wiggle_component.hpp"
#include "tiki/physics_2d_components/physics_2d_body_component.hpp"
#include "tiki/physics_2d_components/physics_2d_collider_component.hpp"
#include "tiki/physics_2d_components/physics_2d_joint_component.hpp"

#include "entities.hpp"
#include "mechanica_level.hpp"

namespace tiki
{
	class Renderer2d;
	class ResourceRequestPool;
	struct InputEvent;
	struct Renderer2dRenderParameters;

	struct GameClientUpdateContext
	{
		GameTime	gameTime;
	};

	enum GameClientLoadResult
	{
		GameClientLoadResult_InProcess,
		GameClientLoadResult_Error,
		GameClientLoadResult_Successful
	};

	class GameClient
	{
		TIKI_NONCOPYABLE_CLASS( GameClient );

	public:

														GameClient();
														~GameClient();

		bool											create();
		void											dispose();

		void											startLoadLevel( ResourceRequestPool& requestPool, string levelName );
		GameClientLoadResult							finalizeLoadLevel( ResourceRequestPool& requestPool );
		void											unloadLevel( ResourceRequestPool& requestPool );

		EntityId										createPlayerEntity( const Vector2& position );
		EntityId										createEntityFromTemplate( const EntityTemplateData& entityTemplate );
		//EntityId										createBoxEntity( const Model* pModel, const Vector3& position );
		//EntityId										createCoinEntity( const Model* pModel, const Vector3& position );
		//EntityId										createTerrainEntity( const Model* pModel, const Vector3& position );

		void											disposeEntity( EntityId entityId );

		void											applyRenderParameters( Renderer2dRenderParameters& renderParameters );

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
		Physics2dWorld					m_physicsWorld;

		const LevelGenericDataResource*	m_pLevel;

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

		void							createLevelEntities();
	};
}
