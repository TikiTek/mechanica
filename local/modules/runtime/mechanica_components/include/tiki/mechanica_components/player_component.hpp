#pragma once

#include "tiki/components/component.hpp"

#include "tiki/math/vector.hpp"

namespace tiki
{
	class Camera;
	class Physics2dCollisionObject;
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

		void				update( double timeDelta );

		bool				processInputEvent( const InputEvent& inputEvent );

	protected:

		virtual bool		internalInitializeState( const ComponentEntityIterator& componentIterator, PlayerComponentState* pComponentState, const PlayerComponentInitData* pComponentInitData ) TIKI_OVERRIDE_FINAL;
		virtual void		internalDisposeState( PlayerComponentState* pComponentState ) TIKI_OVERRIDE_FINAL;

	private:

		const Transform2dComponent*	m_pTransformComponent;

		Physics2dWorld*				m_pPhysicsWorld;

		void						updateDistance( PlayerComponentState* pState ) const;
		void						updateMovement( PlayerComponentState* pState ) const;

		static bool					distanceRayCastCallback( const Physics2dCollisionObject* pObject, const Vector2& point, void* pUserData );
	};
}
