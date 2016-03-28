#include "tiki/editorbase/editorcamera.hpp"

#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/input/inputevent.hpp"
#include "tiki/math/intersection.hpp"
#include "tiki/runtimeshared/freecamera.hpp"

namespace tiki
{
	EditorCamera::EditorCamera()
	{
		m_viewportWidth		= 0;
		m_viewportHeight	= 0;
		m_mousePosX			= 0;
		m_mousePosY			= 0;
	}

	bool EditorCamera::create( GraphicsSystem& graphicsSystem )
	{
		Projection projection;
		projection.createPerspective( (float)graphicsSystem.getBackBuffer().getWidth(), (float)graphicsSystem.getBackBuffer().getHeight(), f32::piOver4, 0.001f, 1000.0f );
				
		const Vector3 position = { 0.0f, 0.0f, 3.0f };
		m_camera.create( position, Quaternion::identity, &projection );
		m_controller.create( position, Quaternion::identity );
		
		return true;
	}

	void EditorCamera::dispose()
	{		
	}

	void EditorCamera::update( double timeDelta )
	{
		m_controller.update( m_camera, timeDelta );
		m_camera.getCameraRay( m_ray, (float)m_mousePosX, (float)m_mousePosY, (float)m_viewportWidth, (float)m_viewportHeight );
	}

	void EditorCamera::resize( int x, int y )
	{
		m_viewportWidth  = x;
		m_viewportHeight = y;

		Projection projection;
		projection.createPerspective( float( m_viewportWidth ), float( m_viewportHeight ), f32::piOver4, 0.001f, 1000.0f );

		m_camera.setProjection( projection );
	}

	bool EditorCamera::processInputEvent( const InputEvent& inputEvent )
	{
		if ( inputEvent.eventType == InputEventType_Mouse_Moved )
		{
			m_mousePosX = inputEvent.data.mouseMoved.xState;
			m_mousePosY = inputEvent.data.mouseMoved.yState;
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