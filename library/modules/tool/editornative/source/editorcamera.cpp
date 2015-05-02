
#include "tiki/editornative/editorcamera.hpp"

#include "tiki/input/inputevent.hpp"
#include "tiki/math/camera.hpp"
#include "tiki/runtimeshared/freecamera.hpp"

namespace tiki
{
	EditorCamera::EditorCamera()
	{
		m_pCamera		= nullptr;
		m_pController	= nullptr;
	}

	bool EditorCamera::create()
	{
		Projection projection;
		projection.createPerspective( 800 / 600, f32::piOver4, 0.001f, 1000.0f );

		const Vector3 position = { 0.0f, 0.0f, 3.0f };
		const Quaternion rotation = Quaternion::identity;
		m_pCamera = new Camera();
		m_pCamera->create( position, rotation, &projection );
		
		m_pController = new FreeCamera();	
		m_pController->create( position, Quaternion::identity );
		
		return true;
	}

	void EditorCamera::dispose()
	{		
		delete m_pCamera;

		if ( m_pController != nullptr )
		{
			m_pController->dispose();
			delete m_pController;
		}
	}

	void EditorCamera::update( float timeDelta )
	{
		m_pController->update( *m_pCamera, timeDelta );
	}

	bool EditorCamera::processInputEvent( InputEvent& inputEvent )
	{
		if ( inputEvent.eventType == InputEventType_Mouse_ButtonDown && inputEvent.data.mouseButton.button == MouseButton_Right )
		{
			m_pController->setMouseControl( true );
			return true;
		}
		else if ( inputEvent.eventType == InputEventType_Mouse_ButtonUp && inputEvent.data.mouseButton.button == MouseButton_Right )
		{
			m_pController->setMouseControl( false );
			return true;
		}

		return m_pController->processInputEvent( inputEvent );
	}

	const Camera& EditorCamera::getCamera()
	{
		return *m_pCamera;
	}
}