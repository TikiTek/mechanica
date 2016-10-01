#pragma once
#ifndef __TIKI_PLAYERCONTROLCOMPONENT_HPP_INCLUDED__
#define __TIKI_PLAYERCONTROLCOMPONENT_HPP_INCLUDED__

#include "tiki/components/component.hpp"
#include "tiki/math/quaternion.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	class Camera;
	class GameCamera;
	class Physics3dCharacterControllerComponent;
	class TransformComponent;
	struct InputEvent;
	struct PlayerControlComponentInitData;
	struct PlayerControlComponentState;

	struct PlayerViewState
	{
		Vector3		eyePosition;
		Quaternion	eyeDirection;
	};

	class PlayerControlComponent : public Component< PlayerControlComponentState, PlayerControlComponentInitData >
	{
		TIKI_NONCOPYABLE_CLASS( PlayerControlComponent );

	public:

		explicit			PlayerControlComponent();
		virtual				~PlayerControlComponent();

		bool				create( const TransformComponent& transformComponent, const Physics3dCharacterControllerComponent& physicsCharacterControllerComponent );
		void				dispose();

		void				update( const GameCamera& gameCamera, const Camera& camera, float timeDelta );
		
		bool				processInputEvent( const InputEvent& inputEvent );

		void				getPlayerViewState( PlayerViewState& rTargetState, const PlayerControlComponentState* pState ) const;

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

		const TransformComponent*						m_pTransformComponent;
		const Physics3dCharacterControllerComponent*	m_pPhysicsCharacterControllerComponent;

		PlayerInputState								m_inputState;

	};
}

#endif // __TIKI_PLAYERCONTROLCOMPONENT_HPP_INCLUDED__
