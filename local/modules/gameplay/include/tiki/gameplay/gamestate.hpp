#pragma once
#ifndef __TIKI_GAMESTATE_HPP_INCLUDED__
#define __TIKI_GAMESTATE_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/components/component_types.hpp"
#include "tiki/math/vector.hpp"
#include "tiki/base/fixedsizedarray.hpp"

namespace tiki
{
	class Animation;
	class GameClient;
	class Model;
	class PhysicsShape;
	class ResourceManager;
	struct FrameData;
	struct PhysicsCharacterControllerComponentState;
	struct Vector3;

	typedef FixedSizedArray< EntityId, 4u > CollectedCoinIdArray;

	class GameState
	{
		TIKI_NONCOPYABLE_CLASS( GameState );

	public:

		GameState();
		~GameState();

		bool		create( GameClient& gameClient, ResourceManager& resourceManager );
		void		dispose( ResourceManager& resourceManager );

		void		update( FrameData& frameData, float timeDelta, float totalGameTime );
		void		render() const;

		void		processCollectedCoins( const CollectedCoinIdArray& collectedCoins );

		EntityId	getPlayerEntityId() const { return m_playerEntityId; }

	private:

		enum
		{
			MaxBoxCount						= 128u,
			MaxCoinCount					= 128u,
			MaxCollectedCoinCount			= 8u,

			BoxSpawnIntervalMilliseconds	= 1000u,
			CoinSpawnIntervalMilliseconds	= 1000u
		};

		struct CollectedCoinState
		{
			Vector3		position;
			float		timeToLife;
		};
		typedef FixedSizedArray< CollectedCoinState, MaxCollectedCoinCount > CollectedCoinStateArray;

		GameClient*				m_pGameClient;

		const Model*			m_pModelBox;
		const Model*			m_pModelCoin;
		const Model*			m_pModelPlane;
		const Model*			m_pModelPlayer;
		const Animation*		m_pAnimationPlayer;
		
		EntityId				m_planeEntityId;

		CollectedCoinStateArray	m_collectedCoins;

		float					m_lastBoxSpawn;
		float					m_lastCoinSpawn;
		
		EntityId									m_playerEntityId;
		PhysicsCharacterControllerComponentState*	m_pPlayerPhysicsControllerState;

		bool					findPositionForShape( Vector3& position, float y, const PhysicsShape& shape );

		void					spawnBox();
		void					spawnCoin();

	};
}

#endif // __TIKI_GAMESTATE_HPP_INCLUDED__
