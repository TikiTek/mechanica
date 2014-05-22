#pragma once
#ifndef __TIKI_GAMESTATE_HPP_INCLUDED__
#define __TIKI_GAMESTATE_HPP_INCLUDED__

#include "tiki/base/sizedarray.hpp"
#include "tiki/base/types.hpp"
#include "tiki/components/component_types.hpp"

namespace tiki
{
	class GameClient;

	class GameState
	{
		TIKI_NONCOPYABLE_CLASS( GameState );

	public:

		GameState();
		~GameState();

		void		create( GameClient& gameClient );
		void		dispose();

		void		update( float totalGameTime );
		void		render() const;

	private:

		enum
		{
			MaxBoxCount					= 128u,
			MaxCoinCount				= 128u,

			BoxSpawnIntervalSeconds		= 5u,
			CoinSpawnIntervalSeconds	= 5u
		};

		GameClient*				m_pGameClient;

		EntityId									m_playerEntityId;
		PhysicsCharacterControllerComponentState*	m_pPlayerPhysicsControllerState;

		SizedArray< EntityId >	m_boxEntityIds;
		SizedArray< EntityId >	m_coinEntityIds;

		float					m_lastBoxSpawn;
		float					m_lastCoinSpawn;

	};
}

#endif // __TIKI_GAMESTATE_HPP_INCLUDED__
