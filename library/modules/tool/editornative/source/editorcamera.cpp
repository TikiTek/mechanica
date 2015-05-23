
#include "tiki/editornative/editorcamera.hpp"

#include "tiki/input/inputevent.hpp"
#include "tiki/math/camera.hpp"
#include "tiki/runtimeshared/freecamera.hpp"
#include "tiki/math/intersection.hpp"

namespace tiki
{
	EditorCamera::EditorCamera()
	{
		m_pCamera			= nullptr;
		m_pController		= nullptr;
		m_ViewportWidth		= 0;
		m_ViewportHeight	= 0;
		m_MousePosX			= 0;
		m_MousePosY			= 0;
		m_Ray				= nullptr;
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
		
		m_Ray = new Ray3();

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

		delete m_Ray;
	}

	void EditorCamera::update( float timeDelta )
	{
		m_pController->update( *m_pCamera, timeDelta );
		m_pCamera->getCameraRay( m_MousePosX, m_MousePosY, m_ViewportWidth, m_ViewportHeight, m_Ray );
	}

	void EditorCamera::resize( int x, int y )
	{
		m_ViewportWidth  = x;
		m_ViewportHeight = y;
	}

	bool EditorCamera::processInputEvent( InputEvent& inputEvent )
	{
		if ( inputEvent.eventType == InputEventType_Mouse_Moved )
		{
			m_MousePosX = inputEvent.data.mouseMoved.xState;
			m_MousePosY = inputEvent.data.mouseMoved.yState;
		}

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

	const Ray3& EditorCamera::getMouseRay()
	{
		return *m_Ray;
	}

}