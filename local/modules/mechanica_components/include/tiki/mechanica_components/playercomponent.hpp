#pragma once
#ifndef TIKI_PLAYERCOMPONENT_HPP_INCLUDED
#define TIKI_PLAYERCOMPONENT_HPP_INCLUDED

#include "tiki/components/component.hpp"

#include "tiki/math/vector.hpp"

namespace tiki
{
	class Camera;
	class Physics2dWorld;
	class Transform2dComponent;
	struct InputEvent;
	struct PlayerComponentInitData;
	struct PlayerComponentState;

	class PlayerComponent : public Component< PlayerComponentState, PlayerComponentInitData >
	{
		TIKI_NONCOPYABLE_CLASS( PlayerComponent );

	public:

		explicit			PlayerComponent();
		virtual				~PlayerComponent();

		bool				create( Physics2dWorld& physicsWorld, const Transform2dComponent& transformComponent );
		void				dispose();

		void				update( float timeDelta );
		
		bool				processInputEvent( const InputEvent& inputEvent );

	protected:

		virtual bool		internalInitializeState( ComponentEntityIterator& componentIterator, PlayerComponentState* pComponentState, const PlayerComponentInitData* pComponentInitData ) TIKI_OVERRIDE_FINAL;
		virtual void		internalDisposeState( PlayerComponentState* pComponentState ) TIKI_OVERRIDE_FINAL;

	private:

		const Transform2dComponent*	m_pTransformComponent;

		Physics2dWorld*				m_pPhysicsWorld;

		void						updateMovement( PlayerComponentState* pState );
	};
}

#endif // TIKI_PLAYERCOMPONENT_HPP_INCLUDED
