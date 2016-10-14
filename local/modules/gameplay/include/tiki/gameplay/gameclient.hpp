#pragma once
#ifndef TIKI_GAMECLIENT_HPP__INCLUDED
#define TIKI_GAMECLIENT_HPP__INCLUDED

#include "tiki/components/lifetimecomponent.hpp"
#include "tiki/components3d/skinnedmodelcomponent.hpp"
#include "tiki/components3d/staticmodelcomponent.hpp"
#include "tiki/components3d/terraincomponent.hpp"
#include "tiki/components3d/transform3dcomponent.hpp"
#include "tiki/entitysystem/entitysystem.hpp"
#include "tiki/gamecomponents/coincomponent.hpp"
#include "tiki/gamecomponents/playercontrolcomponent.hpp"
#include "tiki/gameplay/gamecamera.hpp"
#include "tiki/physics3d/physics3dworld.hpp"
#include "tiki/physics3dcomponents/physics3dbodycomponent.hpp"
#include "tiki/physics3dcomponents/physics3dcharactercontrollercomponent.hpp"
#include "tiki/physics3dcomponents/physics3dcollidercomponent.hpp"
#include "tiki/renderer/renderscene.hpp"
#include "tiki/runtimeshared/freecamera.hpp"

namespace tiki
{
	class GameRenderer;
	class Model;
	struct InputEvent;
	struct ViewData;

	struct GameClientUpdateContext
	{
		GameClientUpdateContext()
		{
			pPlayerCollider = nullptr;
			pTerrainState	= nullptr;
		}

		float							totalGameTime;
		float							timeDelta;

		const Physics3dCollisionObject*	pPlayerCollider;
		const TerrainComponentState*	pTerrainState;

		CollectedCoinIdArray			collectedCoins;
	};

	class GameClient
	{
		TIKI_NONCOPYABLE_CLASS( GameClient );

	public:

														GameClient();
														~GameClient();

		bool											create();
		void											dispose();

		EntityId										createPlayerEntity( const Model* pModel, const Vector3& position );
		EntityId										createModelEntity( const Model* pModel, const Vector3& position );
		EntityId										createBoxEntity( const Model* pModel, const Vector3& position );
		EntityId										createCoinEntity( const Model* pModel, const Vector3& position );
		EntityId										createTerrainEntity( const Model* pModel, const Vector3& position );

		void											disposeEntity( EntityId entityId );

		void											update( GameClientUpdateContext& updateContext );
		void											render( GameRenderer& gameRenderer, GraphicsContext& graphicsContext );

		bool											processInputEvent( const InputEvent& inputEvent );

		EntitySystem&									getEntitySystem()	{ return m_entitySystem; }
		Physics3dWorld&									getPhysicsWorld()	{ return m_physicsWorld; }

		RenderScene&									getScene()			{ return m_renderScene; }
		const RenderScene&								etScene() const		{ return m_renderScene; }
		RenderView&										getView()			{ return *m_pRenderView; }
		const RenderView&								getView() const		{ return *m_pRenderView; }

		const Physics3dCharacterControllerComponent&	getPhysicsCharacterControllerComponent() const { return m_physicsCharacterControllerComponent; }
		const PlayerControlComponent&					getPlayerControlComponent() const { return m_playerControlComponent; }
		const Transform3dComponent&						getTransformComponent() const { return m_transformComponent; }
		const TerrainComponent&							getTerrainComponent() const { return m_terrainComponent; }

	private:

		enum 
		{
			MaxTypeCount	= 16u,
			ChunkCount		= 128u,
			ChunkSize		= 4096u
		};

		EntitySystem							m_entitySystem;

		Physics3dWorld							m_physicsWorld;

		RenderScene								m_renderScene;
		RenderView*								m_pRenderView;

		GameCamera								m_gameCamera;
		FreeCamera								m_freeCamera;

		LifeTimeComponent						m_lifeTimeComponent;
		Physics3dBodyComponent					m_physicsBodyComponent;
		Physics3dCharacterControllerComponent	m_physicsCharacterControllerComponent;
		Physics3dColliderComponent				m_physicsColliderComponent;
		SkinnedModelComponent					m_skinnedModelComponent;
		StaticModelComponent					m_staticModelComponent;
		TerrainComponent						m_terrainComponent;
		Transform3dComponent					m_transformComponent;

		PlayerControlComponent					m_playerControlComponent;
		CoinComponent							m_coinComponent;

	};
}

#endif // TIKI_GAMECLIENT_HPP__INCLUDED
