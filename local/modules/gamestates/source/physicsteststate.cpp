#include "tiki/gamestates/physicsteststate.hpp"

#include "tiki/physics2d/physics2dboxshape.hpp"
#include "tiki/graphics/color_xkcd.hpp"
#include "tiki/debugrenderer/debugrenderer.hpp"

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

	TransitionState PhysicsTestState::processTransitionStep( size_t currentStep, bool isCreating, bool isInital )
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

				m_world.create( vector::create( 0.0f, 9.81f ) );

				Physics2dBoxShape boxShape;
				boxShape.create( vector::create( 200.0f, 20.0f ) );
				
				TIKI_VERIFY( m_collider.create( m_world, boxShape, vector::create( 0.0f, 25.0f ) ) );

				boxShape.create( vector::create( 5.0f, 5.0f ) );
				TIKI_VERIFY( m_body.create( m_world, boxShape, vector::create( 0.0f, 0.0f ), 1.0f, 1.0f ) );

				return TransitionState_Finish;
			}
			else
			{
				TIKI_ASSERT( isInital );

				m_collider.dispose( m_world );
				m_body.dispose( m_world );

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

		debugrenderer::drawLine( vector::create( 0.0f, 0.0f, 0.0f ), vector::create( 100.0f, 100.0f, 0.0f ), TIKI_COLOR_RED );

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		m_world.renderDebug();
#endif
	}

	bool PhysicsTestState::processInputEvent( const InputEvent& inputEvent )
	{
		if( inputEvent.eventType == InputEventType_Mouse_ButtonDown && inputEvent.data.mouseButton.button == MouseButton_Right )
		{
			m_mouseRightState = true;
		}
		else if( inputEvent.eventType == InputEventType_Mouse_ButtonUp && inputEvent.data.mouseButton.button == MouseButton_Right )
		{
			m_mouseRightState = false;
		}
		else if( inputEvent.eventType == InputEventType_Mouse_Moved && m_mouseRightState )
		{
			Vector3 cameraPosition = m_camera.getPosition();
			cameraPosition.x -= inputEvent.data.mouseMoved.xOffset;
			cameraPosition.y -= inputEvent.data.mouseMoved.yOffset;

			m_camera.setTransform( cameraPosition, Quaternion::identity );
		}
		else if ( inputEvent.eventType == InputEventType_Keyboard_Down )
		{
			switch ( inputEvent.data.keybaordKey.key )
			{
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

		return false;
	}
}