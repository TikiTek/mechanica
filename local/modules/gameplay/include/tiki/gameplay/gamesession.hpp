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

		EntityId	getPlayerEntityId() const { return m_playerEntityId; }

	private:

		enum
		{
			MaxCoinCount					= 128u,
		};


		GameClient*				m_pGameClient;

		const Model*			m_pModelPlayer;
		const Model*			m_pModelTerrain;
		const Model*			m_pModelCoin;
		
		EntityId				m_planeEntityId;
		EntityId				m_playerEntityId;

	};
}

#endif // __TIKI_GAMESTATE_HPP_INCLUDED__
