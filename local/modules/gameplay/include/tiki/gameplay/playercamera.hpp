#pragma once
#ifndef __TIKI_PLAYERCAMERA_HPP_INCLUDED__
#define __TIKI_PLAYERCAMERA_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/components/component_types.hpp"

namespace tiki
{
	class Camera;
	class PlayerControlComponent;
	struct PlayerControlComponentState;

	class PlayerCamera
	{
		TIKI_NONCOPYABLE_CLASS( PlayerCamera );

	public:

		PlayerCamera();
		~PlayerCamera();

		void		create( EntityId playerEntityId, const PlayerControlComponentState* pControlState, const PlayerControlComponent& playerControlComponent );
		void		dispose();

		void		update( Camera& targetCamera );
		
	private:

		const PlayerControlComponent*		m_pControlComponent;

		EntityId							m_playerEntityId;
		const PlayerControlComponentState*	m_pControlState;

	};
}

#endif // __TIKI_PLAYERCAMERA_HPP_INCLUDED__
