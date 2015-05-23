
#include "tiki/editornative/transformgizmo.hpp"

#include "tiki/math/vector.hpp"
#include "tiki/math/quaternion.hpp"
#include "tiki/graphics/immediaterenderer.hpp"
#include "tiki/editornative/editorcamera.hpp"
#include "tiki/input/inputevent.hpp"
#include "tiki/math/box.hpp"

namespace tiki
{
	TransformGizmo::TransformGizmo()
		: m_GizmoMode( GizmoMode::None ),
		m_SelectedAxis( AxisType::None )
	{
		m_LineOffset			= 1.0f;
		m_LineLength			= 3.0f;
		m_MultiAxisThickness	= 0.05f;
		m_SingleAxisThickness	= 0.2f;
		m_pAxisBounds			= nullptr;
		m_LeftMouseDown			= false;

		m_pPosition = nullptr;
		m_pRotation = nullptr;
	}

	TransformGizmo::~TransformGizmo()
	{

	}

	bool TransformGizmo::create( ImmediateRenderer* renderer, EditorCamera^ camera )
	{
		m_pRenderer = renderer;
		m_pCamera	= camera;

		m_pAxisBounds = new Box[ AxisType::Count ];
		
		m_pAxisBounds[ AxisType::X  ].create( m_LineOffset, 0, 0, m_LineOffset + m_LineLength, m_SingleAxisThickness, m_SingleAxisThickness );
		m_pAxisBounds[ AxisType::Y  ].create( 0, m_LineOffset, 0, m_SingleAxisThickness, m_LineOffset + m_LineLength, m_SingleAxisThickness );
		m_pAxisBounds[ AxisType::Z  ].create( 0, 0, m_LineOffset, m_SingleAxisThickness, m_SingleAxisThickness, m_LineOffset + m_LineLength );

		m_pAxisBounds[ AxisType::XZ ].create( 0, 0, 0, m_LineOffset, m_MultiAxisThickness, m_LineOffset);
		m_pAxisBounds[ AxisType::XY ].create( 0, 0, 0, m_LineOffset, m_LineOffset, m_MultiAxisThickness );
		m_pAxisBounds[ AxisType::YZ ].create( 0, 0, 0, m_MultiAxisThickness, m_LineOffset, m_LineOffset );

		m_pPosition = new Vector3();
		*m_pPosition = Vector3::zero;

		m_pRotation = new Quaternion();
		*m_pRotation = Quaternion::identity;

		return true;
	}

	void TransformGizmo::dispose()
	{
		if ( m_pAxisBounds )
		{
			delete[] m_pAxisBounds;
			m_pAxisBounds = nullptr;
		}
	}

	bool TransformGizmo::processInputEvent( InputEvent& inputEvent )
	{
		if ( inputEvent.eventType == InputEventType_Mouse_ButtonDown && inputEvent.data.mouseButton.button == MouseButton_Left )
		{
			m_LeftMouseDown = true;
		}
		if ( inputEvent.eventType == InputEventType_Mouse_ButtonUp && inputEvent.data.mouseButton.button == MouseButton_Left )
		{
			m_LeftMouseDown = false;
		}


		return true;
	}

	void TransformGizmo::update()
	{
		pickAxis();

		if ( m_LeftMouseDown )
		{
			switch ( m_SelectedAxis )	
			{
			case AxisType::X:
			case AxisType::XY:
				{
					Plane plane;

					plane.create( Vector3::forward, m_pPosition->z );

					Vector3 intersection;
					if ( intersection::intersectRayPlane( m_pCamera->getMouseRay(), plane, intersection ) )
					{
						bool tst = true;
					}

				}
				break;
			case AxisType::Z:
			case AxisType::YZ:
			case AxisType::Y:
				break;
			case AxisType::XZ:
				break;
			case AxisType::None:
				break;
			case AxisType::Count:
				TIKI_ASSERT( false );
				break;
			default:
				TIKI_ASSERT( false );
				break;
			}
		}
	}


	void TransformGizmo::pickAxis()
	{
		Ray3 ray = m_pCamera->getMouseRay();

		Vector3 intersection;

		bool picked = false;
		for ( int i = 0; i < AxisType::Count; ++i )
		{
			AxisType current = (AxisType)i;
			if ( intersection::intersectRayBox( ray, m_pAxisBounds[ current ], intersection ) )
			{
				m_SelectedAxis = current;
				picked = true;
			}
		}

		if ( !picked )
		{
			m_SelectedAxis = AxisType::None;
		}
	}


	void TransformGizmo::render()
	{
		m_pRenderer->drawGrid( 2, 10, TIKI_COLOR_GRAY);

		for ( int i = 0; i < AxisType::Count; ++i )
		{
			if ( m_SelectedAxis == (AxisType)i )
			{
				m_pRenderer->drawBox( m_pAxisBounds[ i ], TIKI_COLOR_GREEN);
			}
			else
			{
				m_pRenderer->drawBox( m_pAxisBounds[ i ] );
			}
		}

		m_pRenderer->drawAxes( m_LineLength, m_LineOffset );
	}

}