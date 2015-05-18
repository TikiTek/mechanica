#pragma once
#ifndef __TIKI_PLAYERCAMERA_HPP_INCLUDED__
#define __TIKI_PLAYERCAMERA_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/components/component_types.hpp"

namespace tiki
{
	class Camera;
	class TransformComponent;
	struct TransformComponentState;

	class GameCamera
	{
		TIKI_NONCOPYABLE_CLASS( GameCamera );

	public:

		GameCamera();
		~GameCamera();

		void		create( EntityId playerEntityId, const TransformComponentState* pControlState, const TransformComponent& playerControlComponent );
		void		dispose();

		void		update( Camera& targetCamera );
		
	private:

		const TransformComponent*		m_pTransformComponent;

		EntityId						m_playerEntityId;
		const TransformComponentState*	m_pPlayerState;

	};
}

#endif // __TIKI_PLAYERCAMERA_HPP_INCLUDED__
