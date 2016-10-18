#pragma once
#ifndef TIKI_PLAYERCONTROLCOMPONENT_HPP_INCLUDED
#define TIKI_PLAYERCONTROLCOMPONENT_HPP_INCLUDED

#include "tiki/components/component.hpp"

#include "tiki/math/vector.hpp"

namespace tiki
{
	class Camera;
	class Physics2dWorld;
	class Transform2dComponent;
	struct InputEvent;
	struct PlayerControlComponentInitData;
	struct PlayerControlComponentState;

	class PlayerControlComponent : public Component< PlayerControlComponentState, PlayerControlComponentInitData >
	{
		TIKI_NONCOPYABLE_CLASS( PlayerControlComponent );

	public:

		explicit			PlayerControlComponent();
		virtual				~PlayerControlComponent();

		bool				create( Physics2dWorld& physicsWorld, const Transform2dComponent& transformComponent );
		void				dispose();

		void				update( Camera& camera, float timeDelta );
		
		bool				processInputEvent( const InputEvent& inputEvent );

		virtual crc32		getTypeCrc() const;
		virtual uint32		getStateSize() const;
		virtual const char*	getTypeName() const;

	protected:

		virtual bool		internalInitializeState( ComponentEntityIterator& componentIterator, PlayerControlComponentState* pComponentState, const PlayerControlComponentInitData* pComponentInitData );
		virtual void		internalDisposeState( PlayerControlComponentState* pComponentState );

	private:

		const Transform2dComponent*	m_pTransformComponent;
		const Physics2dWorld*		m_pPhysicsWorld;
	};
}

#endif // TIKI_PLAYERCONTROLCOMPONENT_HPP_INCLUDED
