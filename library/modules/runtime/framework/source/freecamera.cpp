
#include "tiki/framework/freecamera.hpp"

#include "tiki/input/inputevent.hpp"
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

		vector::clear( m_leftStickState );
		vector::clear( m_rightStickState );
		m_leftTriggerState	= 0.0f;
		m_rightTriggerState	= 0.0f;
	}

	void FreeCamera::dispose()
	{
		vector::clear( m_position );
		vector::clear( m_rotation );
	}

	void FreeCamera::update( Camera& targetCamera, float timeDelta )
	{
		Quaternion cameraRotation;

		// rotate camera
		{
			Vector2 rotation = m_rightStickState;
			if ( m_enableMouse )
			{
				vector::clear( m_rightStickState );
			}
			vector::scale( rotation, timeDelta * 2.0f );

			vector::add( m_rotation, rotation );
			m_rotation.y = f32::clamp( m_rotation.y, -f32::piOver2, f32::piOver2 );

			quaternion::fromYawPitchRoll( cameraRotation, m_rotation.x, m_rotation.y, 0.0f );
		}

		// move camera
		{
			Vector3 cameraForward;
			Vector3 cameraRight;
			Vector3 cameraUp;
			quaternion::getForward( cameraForward, cameraRotation );
			quaternion::getRight( cameraRight, cameraRotation );
			quaternion::getUp( cameraUp, cameraRotation );

			vector::scale( cameraForward,	m_leftStickState.y * timeDelta * m_speed );
			vector::scale( cameraRight,		m_leftStickState.x * timeDelta * m_speed );
			vector::scale( cameraUp,		( -m_leftTriggerState + m_rightTriggerState ) * timeDelta * m_speed );

			vector::add( m_position, cameraForward );
			vector::add( m_position, cameraRight );
			vector::add( m_position, cameraUp );
		}

		targetCamera.setTransform( m_position, cameraRotation );
	}

	bool FreeCamera::processInputEvent( const InputEvent& inputEvent )
	{
		if ( !m_enableMouse )
		{
			if ( inputEvent.eventType == InputEventType_Controller_StickChanged )
			{
				switch ( inputEvent.data.controllerStick.stickIndex )
				{
				case 0u:
					m_leftStickState.x	= inputEvent.data.controllerStick.xState;
					m_leftStickState.y	= inputEvent.data.controllerStick.yState;
					break;

				case 1u:
					m_rightStickState.x	= inputEvent.data.controllerStick.xState;
					m_rightStickState.y	= inputEvent.data.controllerStick.yState;
					break;
				}

				return true;
			}
			else if ( inputEvent.eventType == InputEventType_Controller_TriggerChanged )
			{
				switch ( inputEvent.data.controllerTrigger.triggerIndex )
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
		}
		else
		{
			if ( inputEvent.eventType == InputEventType_Keyboard_Down )
			{
				switch ( inputEvent.data.keybaordKey.key )
				{
				case KeyboardKey_W:
					m_leftStickState.y = 1.0f;
					return true;
					//break;

				case KeyboardKey_A:
					m_leftStickState.x = -1.0f;
					return true;
					//break;

				case KeyboardKey_S:
					m_leftStickState.y = -1.0f;
					return true;
					//break;

				case KeyboardKey_D:
					m_leftStickState.x = 1.0f;
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
				switch ( inputEvent.data.keybaordKey.key )
				{
				case KeyboardKey_W:
					m_leftStickState.y = 0.0f;
					return true;
					//break;

				case KeyboardKey_A:
					m_leftStickState.x = 0.0f;
					return true;
					//break;

				case KeyboardKey_S:
					m_leftStickState.y = 0.0f;
					return true;
					//break;

				case KeyboardKey_D:
					m_leftStickState.x = 0.0f;
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
				m_rightStickState.x = float( inputEvent.data.mouseMoved.xOffset ) * 0.1f;
				m_rightStickState.y = float( inputEvent.data.mouseMoved.yOffset ) * -0.1f;

				return true;
			}
		}

		return false;
	}
}
