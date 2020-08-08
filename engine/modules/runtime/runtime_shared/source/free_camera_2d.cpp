#include "tiki/runtime_shared/free_camera_2d.hpp"

#include "tiki/input/input_event.hpp"

namespace tiki
{
	FreeCamera2D::FreeCamera2D()
	{
		m_position		= Vector2::zero;
		m_zoom			= 1.0f;
		m_mouseMove		= false;
	}

	void FreeCamera2D::update( double timeDelta )
	{

	}

	bool FreeCamera2D::processInputEvent( const InputEvent& inputEvent )
	{
		if( inputEvent.eventType == InputEventType_Mouse_ButtonDown ||
			inputEvent.eventType == InputEventType_Mouse_ButtonUp )
		{
			if( inputEvent.data.mouseButton.button == MouseButton_Right )
			{
				m_mouseMove = (inputEvent.eventType == InputEventType_Mouse_ButtonDown);
				return true;
			}
		}
		else if( inputEvent.eventType == InputEventType_Mouse_Moved && m_mouseMove )
		{
			m_position.x -= (inputEvent.data.mouseMoved.xOffset / m_zoom) / 100.0f;
			m_position.y -= (inputEvent.data.mouseMoved.yOffset / m_zoom) / 100.0f;
			return true;
		}
		else if( inputEvent.eventType == InputEventType_Mouse_Wheel )
		{
			const float factor = 1.0f + float( inputEvent.data.mouseWheel.offset ) / (InputMouseWheelDelta * 10.0f);
			m_zoom *= factor;
			return true;
		}

		return false;
	}
}
