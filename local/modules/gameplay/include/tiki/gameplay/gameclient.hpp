#pragma once
#ifndef TIKI_GAMECLIENT_HPP__INCLUDED
#define TIKI_GAMECLIENT_HPP__INCLUDED

#include "tiki/components/lifetimecomponent.hpp"
#include "tiki/components2d/texturecomponent.hpp"
#include "tiki/components2d/transform2dcomponent.hpp"
#include "tiki/entitysystem/entitysystem.hpp"
#include "tiki/math/camera.hpp"
#include "tiki/mechanica_components/playercontrolcomponent.hpp"
#include "tiki/mechanica_components/wigglecomponent.hpp"
#include "tiki/physics2d/physics2dworld.hpp"
#include "tiki/physics2dcomponents/physics2dbodycomponent.hpp"
#include "tiki/physics2dcomponents/physics2dcollidercomponent.hpp"
#include "tiki/physics2dcomponents/physics2djointcomponent.hpp"
#include "tiki/renderer/renderscene.hpp"
#include "tiki/runtimeshared/freecamera.hpp"

namespace tiki
{
	class GameRenderer;
	class GraphicsContext;
	class Model;
	struct InputEvent;
	struct ViewData;

	struct GameClientUpdateContext
	{
		GameClientUpdateContext()
		{
			pPlayerCollider = nullptr;
		}

		float							totalGameTime;
		float							timeDelta;

		const Physics2dCollisionObject*	pPlayerCollider;
	};

	class GameClient
	{
		TIKI_NONCOPYABLE_CLASS( GameClient );

	public:

														GameClient();
														~GameClient();

		bool											create();
		void											dispose();

		//EntityId										createPlayerEntity( const Model* pModel, const Vector3& position );
		//EntityId										createModelEntity( const Model* pModel, const Vector3& position );
		//EntityId										createBoxEntity( const Model* pModel, const Vector3& position );
		//EntityId										createCoinEntity( const Model* pModel, const Vector3& position );
		//EntityId										createTerrainEntity( const Model* pModel, const Vector3& position );

		void											disposeEntity( EntityId entityId );

		void											update( GameClientUpdateContext& updateContext );
		void											render( GraphicsContext& graphicsContext );

		bool											processInputEvent( const InputEvent& inputEvent );

		EntitySystem&									getEntitySystem()	{ return m_entitySystem; }
		Physics2dWorld&									getPhysicsWorld()	{ return m_physicsWorld; }

		RenderScene&									getScene()			{ return m_renderScene; }
		const RenderScene&								getScene() const	{ return m_renderScene; }
		RenderView&										getView()			{ TIKI_ASSERT( m_pRenderView ); return *m_pRenderView; }
		const RenderView&								getView() const		{ TIKI_ASSERT( m_pRenderView ); return *m_pRenderView; }

	private:

		enum 
		{
			MaxTypeCount	= 16u,
			ChunkCount		= 128u,
			ChunkSize		= 4096u
		};

		EntitySystem							m_entitySystem;

		Physics2dWorld							m_physicsWorld;

		RenderScene								m_renderScene;
		RenderView*								m_pRenderView;

		Camera									m_gameCamera;
		FreeCamera								m_freeCamera;

		LifeTimeComponent						m_lifeTimeComponent;
		Physics2dBodyComponent					m_physicsBodyComponent;
		Physics2dColliderComponent				m_physicsColliderComponent;
		Physics2dJointComponent					m_physicsJointComponent;
		PlayerControlComponent					m_playerControlComponent;
		TextureComponent						m_textureComponent;
		Transform2dComponent					m_transformComponent;
		WiggleComponent							m_wiggleComponent;
	};
}

#endif // TIKI_GAMECLIENT_HPP__INCLUDED
