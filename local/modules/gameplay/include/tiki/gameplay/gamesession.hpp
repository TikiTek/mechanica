#pragma once
#ifndef __TIKI_GAMESTATE_HPP_INCLUDED__
#define __TIKI_GAMESTATE_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/components/component_types.hpp"
#include "tiki/container/fixedsizedarray.hpp"
#include "tiki/gameplay/gameclient.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	class GameClient;
	class GameRenderer;
	class Model;
	class ResourceManager;
	struct ViewData;
	struct InputEvent;
	struct TransformComponentState;

	class GameSession
	{
		TIKI_NONCOPYABLE_CLASS( GameSession );

	public:

									GameSession();
									~GameSession();

		bool						create( ResourceManager& resourceManager );
		void						dispose( ResourceManager& resourceManager );

		void						update( float timeDelta, float totalGameTime );
		void						render( GameRenderer& gameRenderer, GraphicsContext& graphicsContext );

		bool						processInputEvent( const InputEvent& inputEvent );

		EntityId					getPlayerEntityId() const { return m_playerEntityId; }

		GameClient&					getGameClient() { return m_gameClient; }
		const GameClient&			getGameClient() const { return m_gameClient; }

	private:

		GameClient					m_gameClient;

		EntityId					m_playerEntityId;
		TransformComponentState*	m_pPlayerTransformState;
		const Model*				m_pModelPlayer;
		
		const Model*				m_pModelTerrain;
		const Model*				m_pModelCoin;
		
		EntityId					m_planeEntityId;

	};
}

#endif // __TIKI_GAMESTATE_HPP_INCLUDED__
