#pragma once
#ifndef __TIKI_GAMESTATE_HPP_INCLUDED__
#define __TIKI_GAMESTATE_HPP_INCLUDED__

#include "tiki/base/sizedarray.hpp"
#include "tiki/base/types.hpp"
#include "tiki/components/component_types.hpp"

namespace tiki
{
	class Animation;
	class GameClient;
	class Model;
	class ResourceManager;

	class GameState
	{
		TIKI_NONCOPYABLE_CLASS( GameState );

	public:

		GameState();
		~GameState();

		bool		create( GameClient& gameClient, ResourceManager& resourceManager );
		void		dispose( ResourceManager& resourceManager );

		void		update( float totalGameTime );
		void		render() const;

		EntityId	getPlayerEntityId() const { return m_playerEntityId; }

	private:

		enum
		{
			MaxBoxCount						= 128u,
			MaxCoinCount					= 128u,

			BoxSpawnIntervalMilliseconds	= 1000u,
			CoinSpawnIntervalMilliseconds	= 1000u
		};

		GameClient*				m_pGameClient;

		const Model*			m_pModelBox;
		const Model*			m_pModelCoin;
		const Model*			m_pModelPlane;
		const Model*			m_pModelPlayer;
		const Animation*		m_pAnimationPlayer;
		
		EntityId				m_planeEntityId;

		EntityId									m_playerEntityId;
		PhysicsCharacterControllerComponentState*	m_pPlayerPhysicsControllerState;

		SizedArray< EntityId >	m_boxEntityIds;
		SizedArray< EntityId >	m_coinEntityIds;

		float					m_lastBoxSpawn;
		float					m_lastCoinSpawn;

		bool					findPositionForShape( Vector3& position, float y, const PhysicsShape& shape );

		void					spawnBox();
		void					spawnCoin();

	};
}

#endif // __TIKI_GAMESTATE_HPP_INCLUDED__
