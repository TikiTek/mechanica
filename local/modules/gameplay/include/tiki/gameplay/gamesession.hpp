#pragma once
#ifndef __TIKI_GAMESTATE_HPP_INCLUDED__
#define __TIKI_GAMESTATE_HPP_INCLUDED__

#include "tiki/base/fixedsizedarray.hpp"
#include "tiki/base/types.hpp"
#include "tiki/components/component_types.hpp"
#include "tiki/gameplay/gamecamera.hpp"
#include "tiki/math/vector.hpp"
#include "tiki/runtimeshared/freecamera.hpp"

namespace tiki
{
	class GameClient;
	class Model;
	class ResourceManager;
	struct FrameData;
	struct TransformComponentState;

	class GameSession
	{
		TIKI_NONCOPYABLE_CLASS( GameSession );

	public:

		GameSession();
		~GameSession();

		bool		create( GameClient& gameClient, ResourceManager& resourceManager );
		void		dispose( ResourceManager& resourceManager );

		void		update( FrameData& frameData, float timeDelta, float totalGameTime );
		void		render() const;

		bool		processInputEvent( const InputEvent& inputEvent );

		EntityId	getPlayerEntityId() const { return m_playerEntityId; }

	private:

		GameClient*					m_pGameClient;
		
		bool						m_useFreeCamera;
		GameCamera					m_gameCamera;
		FreeCamera					m_freeCamera;

		EntityId					m_playerEntityId;
		TransformComponentState*	m_pPlayerTransformState;
		const Model*				m_pModelPlayer;
		
		const Model*				m_pModelTerrain;
		const Model*				m_pModelCoin;
		
		EntityId					m_planeEntityId;

	};
}

#endif // __TIKI_GAMESTATE_HPP_INCLUDED__
