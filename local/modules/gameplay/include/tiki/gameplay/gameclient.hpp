#pragma once
#ifndef TIKI_GAMECLIENT_HPP__INCLUDED
#define TIKI_GAMECLIENT_HPP__INCLUDED

#include "tiki/components/lifetimecomponent.hpp"
#include "tiki/components/physicsbodycomponent.hpp"
#include "tiki/components/physicscharactercontrollercomponent.hpp"
#include "tiki/components/physicscollidercomponent.hpp"
#include "tiki/components/playercontrolcomponent.hpp"
#include "tiki/components/skinnedmodelcomponent.hpp"
#include "tiki/components/staticmodelcomponent.hpp"
#include "tiki/components/transformcomponent.hpp"
#include "tiki/entitysystem/entitysystem.hpp"
#include "tiki/gamecomponents/coincomponent.hpp"
#include "tiki/physics/physicsworld.hpp"

namespace tiki
{
	class Model;
	struct InputEvent;

	struct GameClientUpdateContext
	{
		GameClientUpdateContext()
		{
			pPlayerCollider = nullptr;
		}

		float							totalGameTime;
		float							timeDelta;

		const PhysicsCollisionObject*	pPlayerCollider;

		CollectedCoinIdArray			collectedCoins;
	};

	class GameClient
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( GameClient );

	public:

		bool										create();
		void										dispose();

		EntityId									createPlayerEntity( const Model* pModel, const Vector3& position );
		EntityId									createModelEntity( const Model* pModel, const Vector3& position );
		EntityId									createBoxEntity( const Model* pModel, const Vector3& position );
		EntityId									createCoinEntity( const Model* pModel, const Vector3& position );
		EntityId									createPlaneEntity( const Model* pModel, const Vector3& position );

		void										disposeEntity( EntityId entityId );

		void										update( GameClientUpdateContext& updateContext );
		void										render( GameRenderer& gameRenderer );

		bool										processInputEvent( const InputEvent& inputEvent );

		EntitySystem&								getEntitySystem()	{ return m_entitySystem; }
		PhysicsWorld&								getPhysicsWorld()	{ return m_physicsWorld; }

		const PhysicsCharacterControllerComponent&	getPhysicsCharacterControllerComponent() const { return m_physicsCharacterControllerComponent; }
		const PlayerControlComponent&				getPlayerControlComponent() const { return m_playerControlComponent; }
		const TransformComponent&					getTransformComponent() const { return m_transformComponent; }

	private:

		enum 
		{
			MaxTypeCount	= 16u,
			ChunkCount		= 128u,
			ChunkSize		= 4096u
		};

		EntitySystem						m_entitySystem;

		PhysicsWorld						m_physicsWorld;

		LifeTimeComponent					m_lifeTimeComponent;
		PhysicsBodyComponent				m_physicsBodyComponent;
		PhysicsCharacterControllerComponent	m_physicsCharacterControllerComponent;
		PhysicsColliderComponent			m_physicsColliderComponent;
		PlayerControlComponent				m_playerControlComponent;
		SkinnedModelComponent				m_skinnedModelComponent;
		StaticModelComponent				m_staticModelComponent;
		TransformComponent					m_transformComponent;

		CoinComponent						m_coinComponent;

	};
}

#endif // TIKI_GAMECLIENT_HPP__INCLUDED
