#include "tiki/mechanica_components/playercomponent.hpp"

#include "tiki/base/debugprop.hpp"
#include "tiki/components/componentstate.hpp"
#include "tiki/components2d/transform2dcomponent.hpp"
#include "tiki/input/inputevent.hpp"
#include "tiki/math/vector.hpp"
#include "tiki/physics2d/physics2dbody.hpp"
#include "tiki/physics2d/physics2dboxshape.hpp"
#include "tiki/physics2d/physics2dcircleshape.hpp"
#include "tiki/physics2d/physics2djoint.hpp"

#include "mechanica_components.hpp"

namespace tiki
{
	TIKI_DEBUGPROP_FLOAT( s_playerBorder, "GameComponents/PlayerBorder", 0.0f, 0.0f, 300.0f );

	struct PlayerInputState
	{
		void clear()
		{
			left	= false;
			right	= false;
			up		= false;
			down	= false;
			jump	= false;
		}

		bool		left;
		bool		right;
		bool		up;
		bool		down;
		bool		jump;
	};

	struct PlayerComponentState : public ComponentState
	{
		Transform2dComponentState*	pTransform;

		bool						jumping;
		bool						sliding;
		bool						onGround;

		float						maxSpeed;
		float						runSpeed;
		float						sideWalkSpeed;
		float						friction;
		float						jumpImpulse;

		Physics2dBody				physicsBox;
		Physics2dBody				physicsCircle;
		Physics2dJoint				physicsJoint;

		PlayerInputState			inputState;
	};
	TIKI_COMPONENT_STATE_CONSTRUCT_FUNCTIONS( PlayerComponentState );

	PlayerComponent::PlayerComponent()
		: Component( MechanicaComponentType_Player, "PlayerComponent", sizeof( PlayerComponentState ), true )
	{
		m_pTransformComponent	= nullptr;
		m_pPhysicsWorld			= nullptr;
	}

	PlayerComponent::~PlayerComponent()
	{
		TIKI_ASSERT( m_pTransformComponent	== nullptr );
		TIKI_ASSERT( m_pPhysicsWorld		== nullptr );
	}

	bool PlayerComponent::create( Physics2dWorld& physicsWorld, const Transform2dComponent& transformComponent )
	{
		TIKI_ASSERT( m_pTransformComponent	== nullptr );
		TIKI_ASSERT( m_pPhysicsWorld		== nullptr );

		m_pPhysicsWorld			= &physicsWorld;
		m_pTransformComponent	= &transformComponent;


		return true;
	}

	void PlayerComponent::dispose()
	{
		m_pTransformComponent	= nullptr;
		m_pPhysicsWorld			= nullptr;
	}

	void PlayerComponent::update( float timeDelta )
	{
		Iterator componentStates = getIterator();
		State* pState = nullptr;
		while ( pState = componentStates.getNext() )
		{
			updateMovement( pState );
		}
	}

	bool PlayerComponent::processInputEvent( const InputEvent& inputEvent )
	{
		State* pState = getIterator().getNext();
		if (pState == nullptr)
		{
			return false;
		}
		
		PlayerInputState& inputState = pState->inputState;
		if (inputEvent.eventType == InputEventType_Keyboard_Up || inputEvent.eventType == InputEventType_Keyboard_Down)
		{
			const bool isDown = (inputEvent.eventType == InputEventType_Keyboard_Down);

			switch (inputEvent.data.keybaordKey.key)
			{
			case KeyboardKey_A:
			case KeyboardKey_Left:
				inputState.left = isDown;
				return true;

			case KeyboardKey_D:
			case KeyboardKey_Right:
				inputState.right = isDown;
				return true;

			case KeyboardKey_W:
			case KeyboardKey_Up:
				inputState.up = isDown;
				return true;

			case KeyboardKey_S:
			case KeyboardKey_Down:
				inputState.down = isDown;
				return true;

			case KeyboardKey_Space:
				inputState.jump = isDown;
				return true;
			}
		}

		return false;
	}

	bool PlayerComponent::internalInitializeState( ComponentEntityIterator& componentIterator, PlayerComponentState* pState, const PlayerComponentInitData* pInitData )
	{
		pState->pTransform = (Transform2dComponentState*)componentIterator.getFirstOfType( m_pTransformComponent->getTypeId() );

		pState->maxSpeed		= pInitData->maxSpeed;
		pState->runSpeed		= pInitData->runSpeed;
		pState->sideWalkSpeed	= pInitData->sideWalkSpeed;
		pState->jumpImpulse		= pInitData->jumpImpulse;
		pState->friction		= pInitData->friction;

		pState->inputState.clear();

		Physics2dBoxShape boxShape;
		boxShape.create( vector::create( 20.0f, 0.2f ) );

		Physics2dCircleShape circleShape;
		circleShape.create( 0.45f );
		boxShape.create( vector::create( 0.9f, 2.4f ) );

		const Vector2& boxPosition = m_pTransformComponent->getPosition( pState->pTransform );

		Vector2 circlePosition = boxPosition;
		circlePosition.y += 1.2f;

		if( !pState->physicsBox.create( *m_pPhysicsWorld, boxShape, boxPosition, pInitData->mass / 2.0f, 0.0f, true ) ||
			!pState->physicsCircle.create( *m_pPhysicsWorld, circleShape, circlePosition, pInitData->mass / 2.0f, pInitData->friction ) ||
			!pState->physicsJoint.createAsRevolute( *m_pPhysicsWorld, pState->physicsBox, vector::create( 0.0f, 1.2f ), pState->physicsCircle, Vector2::zero, true, pInitData->maxMotorTorque ) )
		{
			return false;
		}
		
		return true;
	}

	void PlayerComponent::internalDisposeState( PlayerComponentState* pState )
	{
		pState->physicsJoint.dispose( *m_pPhysicsWorld );
		pState->physicsCircle.dispose( *m_pPhysicsWorld );
		pState->physicsBox.dispose( *m_pPhysicsWorld );
	}

	void PlayerComponent::updateMovement( PlayerComponentState* pState )
	{
		// fall
		if (!pState->jumping && !pState->onGround)
		{
			//this.charakterAnimation.Fall( gameTime );
			// TODO: SoundFall();
		}

		// jump
		if (pState->inputState.jump && pState->onGround)
		{
			pState->physicsJoint.setRevoluteMotorSteed( 0.0f );

			const Vector2 jumpForce = vector::create( 0.0f, pState->jumpImpulse );
			pState->physicsBox.applyLocalLinearImpluse( jumpForce );

			pState->jumping = true;
			// TODO: SoundJump();
		}

		// move
		if (pState->inputState.right)
		{
			pState->physicsJoint.setRevoluteMotorSteed( pState->runSpeed );

			if (!pState->onGround && !pState->sliding)
			{
				Vector2 linearVelocity = pState->physicsBox.getLinearVelocity();
				if (linearVelocity.x < pState->sideWalkSpeed)
				{
					linearVelocity.x += 1.0f;
					pState->physicsBox.setLinearVelocity( linearVelocity );
				}
			}
		}
		else if (pState->inputState.left)
		{
			pState->physicsJoint.setRevoluteMotorSteed( -pState->runSpeed );

			if (!pState->onGround && !pState->sliding)
			{
				Vector2 linearVelocity = pState->physicsBox.getLinearVelocity();
				if (linearVelocity.x > -pState->sideWalkSpeed)
				{
					linearVelocity.x -= 1.0f;
					pState->physicsBox.setLinearVelocity( linearVelocity );
				}
			}
		}
		else
		{
			pState->physicsJoint.setRevoluteMotorSteed( 0.0f );
		}
	}
}
