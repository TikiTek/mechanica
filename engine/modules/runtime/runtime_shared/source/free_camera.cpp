#include "tiki/runtime_shared/free_camera.hpp"

#include "tiki/input/input_event.hpp"
#include "tiki/math/camera.hpp"
#include "tiki/math/quaternion.hpp"

namespace tiki
{
	void FreeCamera::create( const Vector3& position, const Quaternion& rotation )
	{
		m_enableMouse	= false;
		m_speed			= 1.0f;
		m_position		= position;

		Vector3 yprRotation;
		quaternion::toYawPitchRoll( yprRotation, rotation );
		m_rotation = vector::create( yprRotation.x, yprRotation.y );

		vector::clear( m_mouseState );
		vector::clear( m_keyboardState );
		vector::clear( m_leftStickState );
		vector::clear( m_rightStickState );
		m_leftTriggerState	= 0.0f;
		m_rightTriggerState	= 0.0f;
	}

	void FreeCamera::dispose()
	{
	}

	void FreeCamera::update( Camera& targetCamera, double timeDelta )
	{
		Quaternion cameraRotation;

		// rotate camera
		{
			Vector2 rotation = m_rightStickState;
			vector::add( rotation, m_mouseState );
			vector::clear( m_mouseState );
			vector::scale( rotation, float( timeDelta * 2.0 ) );

			vector::add( m_rotation, rotation );
			m_rotation.y = f32::clamp( m_rotation.y, -f32::piOver2, f32::piOver2 );

			quaternion::fromYawPitchRoll( cameraRotation, m_rotation.x, m_rotation.y, 0.0f );
		}

		// move camera
		{
			Vector2 cameraMovement = m_leftStickState;
			vector::add( cameraMovement, m_keyboardState );
			const float upDownState = (-m_leftTriggerState + m_rightTriggerState);

			Vector3 cameraForward;
			Vector3 cameraLeft;
			Vector3 cameraUp;
			quaternion::getForward( cameraForward, cameraRotation );
			quaternion::getLeft( cameraLeft, cameraRotation );
			quaternion::getUp( cameraUp, cameraRotation );

			vector::scale( cameraForward,	float( cameraMovement.y * timeDelta * m_speed ) );
			vector::scale( cameraLeft,		float( cameraMovement.x * timeDelta * m_speed ) );
			vector::scale( cameraUp,		float( upDownState * timeDelta * m_speed ) );

			vector::add( m_position, cameraForward );
			vector::add( m_position, cameraLeft );
			vector::add( m_position, cameraUp );
		}

		targetCamera.setTransform( m_position, cameraRotation );
	}

	bool FreeCamera::processInputEvent( const InputEvent& inputEvent )
	{
		if( inputEvent.eventType == InputEventType_Controller_StickChanged )
		{
			switch( inputEvent.data.controllerStick.stickIndex )
			{
			case 0u:
				m_leftStickState.x = inputEvent.data.controllerStick.xState;
				m_leftStickState.y = inputEvent.data.controllerStick.yState;
				break;

			case 1u:
				m_rightStickState.x = inputEvent.data.controllerStick.xState;
				m_rightStickState.y = inputEvent.data.controllerStick.yState;
				break;
			}

			return true;
		}
		else if( inputEvent.eventType == InputEventType_Controller_TriggerChanged )
		{
			switch( inputEvent.data.controllerTrigger.triggerIndex )
			{
			case 0u:
				m_leftTriggerState = inputEvent.data.controllerTrigger.state;
				break;

			case 1u:
				m_rightTriggerState = inputEvent.data.controllerTrigger.state;
				break;
			}

			return true;
		}

		if ( m_enableMouse )
		{
			if ( inputEvent.eventType == InputEventType_Keyboard_Down )
			{
				switch ( inputEvent.data.keyboardKey.key )
				{
				case KeyboardKey_W:
					m_keyboardState.y = -1.0f;
					return true;
					//break;

				case KeyboardKey_A:
					m_keyboardState.x = -1.0f;
					return true;
					//break;

				case KeyboardKey_S:
					m_keyboardState.y = 1.0f;
					return true;
					//break;

				case KeyboardKey_D:
					m_keyboardState.x = 1.0f;
					return true;
					//break;

				case KeyboardKey_LeftShift:
				case KeyboardKey_RightShift:
					m_speed = 10.0f;
					return true;
					//break;

				default:
					break;
				}
			}
			else if ( inputEvent.eventType == InputEventType_Keyboard_Up )
			{
				switch ( inputEvent.data.keyboardKey.key )
				{
				case KeyboardKey_W:
				case KeyboardKey_S:
					m_keyboardState.y = 0.0f;
					return true;
					//break;

				case KeyboardKey_A:
				case KeyboardKey_D:
					m_keyboardState.x = 0.0f;
					return true;
					//break;

				case KeyboardKey_LeftShift:
				case KeyboardKey_RightShift:
					m_speed = 1.0f;
					return true;
					//break;
				}
			}
			else if ( inputEvent.eventType == InputEventType_Mouse_Moved )
			{
				m_mouseState.x = float( inputEvent.data.mouseMoved.xOffset ) * 0.1f;
				m_mouseState.y = float( inputEvent.data.mouseMoved.yOffset ) * 0.1f;

				return true;
			}
		}

		return false;
	}

	void FreeCamera::setMouseControl( bool enabled )
	{
		m_enableMouse = enabled;
	}
}
