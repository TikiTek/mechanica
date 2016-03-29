#include "tiki/editorbase/editorcamera.hpp"

#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/input/inputevent.hpp"
#include "tiki/math/intersection.hpp"
#include "tiki/runtimeshared/freecamera.hpp"

namespace tiki
{
	EditorCamera::EditorCamera()
	{
	}

	bool EditorCamera::create( GraphicsSystem& graphicsSystem )
	{
		vector::clear( m_mousePosition );

		const Vector3 position = { 0.0f, 0.0f, 3.0f };
		m_camera.create( position, Quaternion::identity );
		m_controller.create( position, Quaternion::identity );

		resize( graphicsSystem.getBackBuffer().getWidth(), graphicsSystem.getBackBuffer().getHeight() );
		
		return true;
	}

	void EditorCamera::dispose()
	{		
	}

	void EditorCamera::update( double timeDelta )
	{
		m_controller.update( m_camera, timeDelta );
		m_camera.getCameraRay( m_ray, m_mousePosition );
	}

	void EditorCamera::resize( uint x, uint y )
	{
		Projection projection;
		projection.createPerspective( float( x ), float( y ), f32::piOver4, 0.001f, 1000.0f );

		m_camera.setProjection( projection );
	}

	bool EditorCamera::processInputEvent( const InputEvent& inputEvent )
	{
		if ( inputEvent.eventType == InputEventType_Mouse_Moved )
		{
			vector::set( m_mousePosition, float( inputEvent.data.mouseMoved.xState ), float( inputEvent.data.mouseMoved.yState ) );
		}

		if ( inputEvent.eventType == InputEventType_Mouse_ButtonDown && inputEvent.data.mouseButton.button == MouseButton_Right )
		{
			m_controller.setMouseControl( true );
			return true;
		}
		else if ( inputEvent.eventType == InputEventType_Mouse_ButtonUp && inputEvent.data.mouseButton.button == MouseButton_Right )
		{
			m_controller.setMouseControl( false );
			return true;
		}

		return m_controller.processInputEvent( inputEvent );
	}
}