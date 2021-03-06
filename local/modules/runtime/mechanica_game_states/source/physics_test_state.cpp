#include "tiki/game_states/physics_test_state.hpp"

#include "tiki/debug_renderer/debug_renderer.hpp"
#include "tiki/graphics/color_xkcd.hpp"
#include "tiki/physics_2d/physics_2d_box_shape.hpp"
#include "tiki/physics_2d/physics_2d_circle_shape.hpp"
#include "tiki/physics_2d/physics_2d_none_shape.hpp"

namespace tiki
{
	PhysicsTestState::PhysicsTestState()
	{
		m_pGame			= nullptr;
	}

	PhysicsTestState::~PhysicsTestState()
	{
		TIKI_ASSERT( m_pGame == nullptr );
	}

	void PhysicsTestState::create( Game* pGame )
	{
		m_pGame			= pGame;
	}

	void PhysicsTestState::dispose()
	{
		m_pGame			= nullptr;
	}

	TransitionState PhysicsTestState::processTransitionStep( uintreg currentStep, bool isCreating, bool isInital )
	{
		switch ( currentStep )
		{
		case PhysicsTestStateTransitionSteps_Initialize:
			if ( isCreating )
			{
				TIKI_ASSERT( isInital );

				m_mouseRightState = false;

				const RenderTarget& backBuffer = m_pGame->getGraphicsSystem().getBackBuffer();

				Projection projection;
				projection.createOrthographic( (float)backBuffer.getWidth(), -(float)backBuffer.getHeight(), 0.0f, 10.0f );

				m_camera.create( Vector3::zero, Quaternion::identity, &projection );

				m_world.create( vector::create( 0.0f, 9.81f ), 100.0f );

				Physics2dBoxShape boxShape;
				boxShape.create( vector::create( 20.0f, 0.2f ) );

				//TIKI_VERIFY( m_collider.create( m_world, boxShape, vector::create( 0.0f, 1.0f ) ) );

				Physics2dCircleShape circleShape;
				circleShape.create( 0.45f );
				boxShape.create( vector::create( 0.9f, 2.4f ) );

				TIKI_VERIFY( m_playerBox.create( m_world, boxShape, vector::create( 0.0f, -1.5f ), 1.0f, 1.0f, true ) );
				TIKI_VERIFY( m_playerCircle.create( m_world, circleShape, vector::create( 0.0f, -0.3f ), 1.0f, 1.0f ) );
				TIKI_VERIFY( m_playerJoint.createAsRevolute( m_world, m_playerBox, vector::create( 0.0f, 1.2f ), m_playerCircle, Vector2::zero, true, 2000.0f ) );

				boxShape.create( vector::create( 5.0f, 0.2f ) );
				TIKI_VERIFY( m_body1.create( m_world, boxShape, vector::create( 0.0f, 0.0f ), 100.0f, 1.0f ) );

				boxShape.create( vector::create( 0.5f, 0.5f ) );
				TIKI_VERIFY( m_body2.create( m_world, boxShape, vector::create( 2.0f, -2.5f ), 1.0f, 1.0f ) );
				//TIKI_VERIFY( m_joint.createAsRope( m_world, m_body1, Vector2::zero, m_body2, Vector2::zero, 1.5f ) );

				const Vector2 position = m_body1.getPosition();

				Physics2dNoneShape bla;
				boxShape.create( vector::create( 0.05f, 0.05f ) );
				for( uint i = 0u; i < TIKI_COUNT( m_colliders ); ++i )
				{
					const float x = (i == 0 || i == 1 ? 3.0f : -3.0f);
					const float y = (i == 0 || i == 2 ? 1.0f : -1.0f);

					const float lx = (i == 0 || i == 1 ? 2.5f : -2.5f);
					const float ly = (i == 0 || i == 2 ? 0.0f : -0.0f);

					const Vector2 colliderPosition = vector::create( x, y );
					const Vector2 localOffst = vector::create( lx, ly );

					Vector2 localPosition;
					vector::add( localPosition, position, localOffst );

					Vector2 distance;
					vector::sub( distance, localPosition, colliderPosition );

					const float ff = (i == 0 || i == 2 ? 1.1f : 1.0f);

					m_colliders[ i ].create( m_world, bla, colliderPosition );
					m_joints[ i ].createAsRope( m_world, m_colliders[ i ], Vector2::zero, m_body1, vector::create( lx, ly ), vector::length( distance ) * ff );
				}

				return TransitionState_Finish;
			}
			else
			{
				TIKI_ASSERT( isInital );

				//m_joint.dispose( m_world );

				m_playerJoint.dispose( m_world );
				m_playerBox.dispose( m_world );
				m_playerCircle.dispose( m_world );

				for( uint i = 0u; i < TIKI_COUNT( m_colliders); ++i )
				{
					m_joints[ i ].dispose( m_world );
					m_colliders[ i ].dispose( m_world );
				}

				m_body2.dispose( m_world );
				m_body1.dispose( m_world );

				//m_collider.dispose( m_world );

				m_world.dispose();

				return TransitionState_Finish;
			}
			//break;

		case PhysicsTestStateTransitionSteps_LoadResources:
			if ( isCreating )
			{
				TIKI_ASSERT( isInital );

				return TransitionState_Finish;
			}
			else
			{
				TIKI_ASSERT( isInital );

				return TransitionState_Finish;
			}
			//break;

		default:
			break;
		}

		TIKI_BREAK( "no result\n" );
		return TransitionState_Error;
	}

	void PhysicsTestState::update()
	{
		const float timeDelta = float( m_pGame->getFrameTimer().getElapsedTime() );

		m_world.update( timeDelta );
	}

	void PhysicsTestState::render( GraphicsContext& graphicsContext )
	{
		graphicsContext.clear( graphicsContext.getBackBuffer(), TIKI_COLOR_XKCD_BRIGHT_BLUE );

		//debugrenderer::drawLine( vector::create( 0.0f, 0.0f, 0.0f ), vector::create( 100.0f, 100.0f, 0.0f ), TIKI_COLOR_RED );

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		m_world.renderDebug();
#endif
	}

	bool PhysicsTestState::processInputEvent( const InputEvent& inputEvent )
	{
		if( inputEvent.eventType == InputEventType_Mouse_ButtonDown && inputEvent.data.mouseButton.button == MouseButton_Right )
		{
			m_mouseRightState = true;
			return true;
		}
		else if( inputEvent.eventType == InputEventType_Mouse_ButtonUp && inputEvent.data.mouseButton.button == MouseButton_Right )
		{
			m_mouseRightState = false;
			return true;
		}
		else if( inputEvent.eventType == InputEventType_Mouse_Moved && m_mouseRightState )
		{
			Vector3 cameraPosition = m_camera.getPosition();
			cameraPosition.x -= inputEvent.data.mouseMoved.xOffset;
			cameraPosition.y -= inputEvent.data.mouseMoved.yOffset;

			m_camera.setTransform( cameraPosition, Quaternion::identity );

			return true;
		}
		else if ( inputEvent.eventType == InputEventType_Keyboard_Down )
		{
			switch ( inputEvent.data.keyboardKey.key )
			{
			case KeyboardKey_Left:
				m_playerJoint.setRevoluteMotorSteed( -14.0f );
				return true;

			case KeyboardKey_Right:
				m_playerJoint.setRevoluteMotorSteed( 14.0f );
				return true;

			case KeyboardKey_Z:
				{
					//Physics2dBoxShape shape;
					//shape.create( vector::create( 0.5f, 0.5f ) );
					//
					//bool test = m_gameClient.getPhysicsWorld().checkIntersection( shape, vector::create( 0.0f, 0.25f, 0.0f ) );
					//
					//shape.dispose();
				}
				break;

			default:
				break;
			}
		}
		else if( inputEvent.eventType == InputEventType_Keyboard_Up )
		{
			switch( inputEvent.data.keyboardKey.key )
			{
			case KeyboardKey_Left:
			case KeyboardKey_Right:
				m_playerJoint.setRevoluteMotorSteed( 0.0f );
				return true;
			}
		}

		return false;
	}
}