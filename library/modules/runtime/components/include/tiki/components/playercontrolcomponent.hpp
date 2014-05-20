#pragma once
#ifndef __TIKI_PLAYERCONTROLCOMPONENT_HPP_INCLUDED__
#define __TIKI_PLAYERCONTROLCOMPONENT_HPP_INCLUDED__

#include "tiki/components/component.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	class PhysicsCharacterControllerComponent;
	class TransformComponent;
	struct InputEvent;
	struct PlayerControlComponentInitData;
	struct PlayerControlComponentState;

	class PlayerControlComponent : public Component< PlayerControlComponentState, PlayerControlComponentInitData >
	{
		TIKI_NONCOPYABLE_CLASS( PlayerControlComponent );

	public:

		explicit			PlayerControlComponent();
		virtual				~PlayerControlComponent();

		bool				create( const TransformComponent& transformComponent, const PhysicsCharacterControllerComponent& physicsCharacterControllerComponent );
		void				dispose();

		void				update( float timeStep );
		
		bool				processInputEvent( const InputEvent& inputEvent );

		virtual crc32		getTypeCrc() const;
		virtual uint32		getStateSize() const;
		virtual const char*	getTypeName() const;

	protected:

		virtual bool		internalInitializeState( ComponentEntityIterator& componentIterator, PlayerControlComponentState* pComponentState, const PlayerControlComponentInitData* pComponentInitData );
		virtual void		internalDisposeState( PlayerControlComponentState* pComponentState );

	private:

		struct PlayerInputState
		{
			Vector2		leftStick;
			Vector2		rightStick;

			bool		jump;
		};

		const TransformComponent*					m_pTransformComponent;
		const PhysicsCharacterControllerComponent*	m_pPhysicsCharacterControllerComponent;

		ComponentTypeId								m_transformComponentTypeId;
		ComponentTypeId								m_physicsCharacterControllerComponentTypeId;

		PlayerInputState							m_inputState;

	};
}

#endif // __TIKI_PLAYERCONTROLCOMPONENT_HPP_INCLUDED__
