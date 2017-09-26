#include "tiki/editorbase/transformgizmo.hpp"

#include "tiki/editorbase/editorcamera.hpp"

#include "tiki/debugrenderer/debugrenderer.hpp"
#include "tiki/graphics/immediaterenderer.hpp"
#include "tiki/input/inputevent.hpp"
#include "tiki/math/box.hpp"
#include "tiki/math/intersection.hpp"
#include "tiki/math/plane.hpp"
#include "tiki/math/quaternion.hpp"
#include "tiki/math/sphere.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	TransformGizmo::TransformGizmo()
	{
		m_gizmoMode				= GizmoMode_None;
		m_selectedAxis			= AxisType_None;
		
		m_lineOffset			= 1.0f;
		m_lineLength			= 3.0f;
		m_multiAxisThickness	= 0.05f;
		m_singleAxisThickness	= 0.2f;
		m_leftMouseDown			= false;
	}

	TransformGizmo::~TransformGizmo()
	{

	}

	bool TransformGizmo::create( const EditorCamera& camera )
	{
		m_pCamera	= &camera;

		m_aAxisBounds[ AxisType_X  ].create( m_lineOffset, 0, 0, m_lineOffset + m_lineLength, m_singleAxisThickness, m_singleAxisThickness );
		m_aAxisBounds[ AxisType_Y  ].create( 0, m_lineOffset, 0, m_singleAxisThickness, m_lineOffset + m_lineLength, m_singleAxisThickness );
		m_aAxisBounds[ AxisType_Z  ].create( 0, 0, m_lineOffset, m_singleAxisThickness, m_singleAxisThickness, m_lineOffset + m_lineLength );
		m_aAxisBounds[ AxisType_XZ ].create( 0, 0, 0, m_lineOffset, m_multiAxisThickness, m_lineOffset);
		m_aAxisBounds[ AxisType_XY ].create( 0, 0, 0, m_lineOffset, m_lineOffset, m_multiAxisThickness );
		m_aAxisBounds[ AxisType_YZ ].create( 0, 0, 0, m_multiAxisThickness, m_lineOffset, m_lineOffset );

		float spherePos = m_lineOffset * 2.0f;
		float radius = 0.5f;

		vector::set( m_aRotateBounds[ 0 ].center, spherePos, 0, 0 );
		m_aRotateBounds[ 0 ].radius = radius;

		vector::set( m_aRotateBounds[ 1 ].center, 0, spherePos, 0 );
		m_aRotateBounds[ 1 ].radius = radius;

		vector::set( m_aRotateBounds[ 2 ].center, 0, 0, spherePos );
		m_aRotateBounds[ 2 ].radius = radius;

		m_position = Vector3::zero;
		m_rotation = Quaternion::identity;
		m_lastIntersection = Vector3::zero;
		m_intersection = Vector3::zero;
		m_delta = Vector3::zero;
		m_translationDelta = Vector3::zero;

		matrix::createIdentity( m_worldMatrix );

		return true;
	}

	void TransformGizmo::dispose()
	{
	}

	bool TransformGizmo::processInputEvent( const InputEvent& inputEvent )
	{
		if ( inputEvent.eventType == InputEventType_Mouse_ButtonDown && inputEvent.data.mouseButton.button == MouseButton_Left )
		{
			m_leftMouseDown = true;
		}
		if ( inputEvent.eventType == InputEventType_Mouse_ButtonUp && inputEvent.data.mouseButton.button == MouseButton_Left )
		{
			m_leftMouseDown = false;
		}

		if ( inputEvent.eventType == InputEventType_Keyboard_Down && inputEvent.data.keybaordKey.key == KeyboardKey_F1)
		{
			m_gizmoMode = GizmoMode_Translate;
		}

		if ( inputEvent.eventType == InputEventType_Keyboard_Up && inputEvent.data.keybaordKey.key == KeyboardKey_F1 )
		{
			m_gizmoMode = GizmoMode_None;
		}

		if ( inputEvent.eventType == InputEventType_Keyboard_Down && inputEvent.data.keybaordKey.key == KeyboardKey_F2 )
		{
			m_gizmoMode = GizmoMode_Scale;
		}

		if ( inputEvent.eventType == InputEventType_Keyboard_Up && inputEvent.data.keybaordKey.key == KeyboardKey_F2 )
		{
			m_gizmoMode = GizmoMode_None;
		}

		if ( inputEvent.eventType == InputEventType_Keyboard_Down && inputEvent.data.keybaordKey.key == KeyboardKey_F3 )
		{
			m_gizmoMode = GizmoMode_Rotate;
		}

		if ( inputEvent.eventType == InputEventType_Keyboard_Up && inputEvent.data.keybaordKey.key == KeyboardKey_F3 )
		{
			m_gizmoMode = GizmoMode_None;
		}

		if ( inputEvent.eventType == InputEventType_Mouse_Moved )
		{
			m_mouseOffsetX = (float)inputEvent.data.mouseMoved.xOffset;
		}

		return true;
	}

	void TransformGizmo::update( double timeDelta )
	{
		pickAxis();

		Vector3 forward = { 0.0f, 0.0f, -1.0f };
		Vector3 left = { -1.0f, 0.0f, 0.0f };
		Vector3 down = { 0.0f, -1.0f, 0.0f };

		// save last intersection and reset 
		m_lastIntersection = m_intersection;
		m_intersection = Vector3::zero;
		m_translationDelta = Vector3::zero;
		quaternion::createIdentity( m_rotation);
		
		if ( m_leftMouseDown && m_selectedAxis != AxisType_None )
		{
			Vector3 delta = Vector3::zero;

			if ( m_gizmoMode == GizmoMode_Translate || m_gizmoMode == GizmoMode_Scale)
			{
				switch ( m_selectedAxis )	
				{
				case AxisType_X:
				case AxisType_XY:
					{
						Plane plane;
						plane.create( forward, f32::abs( m_position.z ) );

						Vector3 intersection;
						if ( intersection::intersectRayPlane( m_pCamera->getMouseRay(), plane, intersection ) )
						{
							m_intersection = intersection;

							if ( !vector::isEquals( m_lastIntersection, Vector3::zero) )
							{
								m_delta = m_intersection;
								vector::sub( m_delta, m_lastIntersection );
							}

							if ( m_selectedAxis == AxisType_X )
							{
								vector::set( delta, m_delta.x, 0.0f, 0.0f );
							}
							else
							{
								vector::set( delta, m_delta.x, m_delta.y, 0.0f );
							}
						}
					}
					break;
				case AxisType_Z:
				case AxisType_YZ:
				case AxisType_Y:
					{
						Plane plane;
						plane.create( left, f32::abs( m_position.x) );

						Vector3 intersection;
						if ( intersection::intersectRayPlane( m_pCamera->getMouseRay(), plane, intersection ) )
						{
							m_intersection = intersection;

							if ( !vector::isEquals( m_lastIntersection, Vector3::zero ) )
							{
								m_delta = m_intersection;
								vector::sub( m_delta, m_lastIntersection );
							}

							if ( m_selectedAxis == AxisType_Y )
							{
								vector::set( delta, 0.0f, m_delta.y, 0.0f );
							}
							else if ( m_selectedAxis == AxisType_Z )
							{
								vector::set( delta, 0.0f, 0.0f, m_delta.z );
							}
							else
							{
								vector::set( delta, 0.0f, m_delta.y, m_delta.z );
							}
						}
					}
					break;
				case AxisType_XZ:
					{
						
						Plane plane;
						plane.create( down, f32::abs( m_position.y ) ); // TODO: Anthony this is shit!

						Vector3 intersection;
						if ( intersection::intersectRayPlane( m_pCamera->getMouseRay(), plane, intersection ) )
						{
							m_intersection = intersection;

							if ( !vector::isEquals( m_lastIntersection, Vector3::zero ) )
							{
								m_delta = m_intersection;
								vector::sub( m_delta, m_lastIntersection );
							}
						}

						vector::set( delta, m_delta.x, 0.0f, m_delta.z );
					}
					break;
				case AxisType_RX:
				case AxisType_RY:
				case AxisType_RZ:
					break;
				case AxisType_None:
				case AxisType_Count:
				default:
					TIKI_ASSERT( false );
					break;
				}

				// vector::Transform(delta, rotation);

				if ( m_gizmoMode == GizmoMode_Translate )
				{
					
					m_translationDelta = delta;

					vector::add( m_position, m_translationDelta );

					m_worldMatrix.pos = m_position;

					if ( !vector::isEquals( m_translationDelta, Vector3::zero ) )
					{
						for ( int i = 0; i < 6; ++i )
						{
							m_aAxisBounds[ i ].translate( m_translationDelta );
						}
						for ( int i = 0; i < 3; ++i )
						{
							m_aRotateBounds[ i ].translate( m_translationDelta );
						}

					}

					// TODO: OnTransformGizmoTranslateEvent(delta)
				}
				else if ( m_gizmoMode == GizmoMode_Scale )
				{
					if ( !vector::isEquals( delta, Vector3::zero ) )
					{
						vector::add( delta, Vector3::one );

						for ( int i = 0; i < AxisType_Count; ++i )
						{
							vector::mul( m_aAxisBounds[ i ].extents, delta );
						}
					}

					// TODO: OnTransformGizmoScaleEvent(delta)
				}

			} // Translate Gizmo mode
			else if ( m_gizmoMode == GizmoMode_Rotate )
			{
				const float delta = float( m_mouseOffsetX * timeDelta );

				Quaternion rotation;

				switch ( m_selectedAxis )
				{
				case AxisType_RX:
					quaternion::fromAxisAngle( rotation, Vector3::unitX, delta );
					break;
				case AxisType_RY:
					quaternion::fromAxisAngle( rotation, Vector3::unitY, delta );
					break;
				case AxisType_RZ:
					quaternion::fromAxisAngle( rotation, forward, delta );
					break;
				case AxisType_X:
				case AxisType_Y:
				case AxisType_Z:
				case AxisType_XZ:
				case AxisType_XY:
				case AxisType_YZ:
					break;
				case AxisType_Count:
				case AxisType_None:
					TIKI_ASSERT( false );
				default:
					break;
				}

				if ( !f32::isZero( delta ) )
				{
					m_rotation = rotation;

					quaternion::toMatrix( m_worldMatrix.rot, rotation );

					for ( int i = 0; i < 3; ++i )
					{
						m_aAxisBounds[ i ].rotate( m_rotation );
					}

					// TODO: OnTransformGizmoRotateEvent(delta)
				}
			}
		}
	}

	void TransformGizmo::pickAxis()
	{
		// don't pick any axis if left mouse button is currently down or dragging the gizmo
		if ( m_leftMouseDown )
		{
			return;
		}

		Vector3 intersection;
		bool picked = false;
		for ( int i = 0; i <= AxisType_YZ; ++i )
		{
			AxisType current = (AxisType)i;
			
			if ( intersection::intersectRayBox( m_pCamera->getMouseRay(), m_aAxisBounds[ current ], intersection ) )
			{
				m_selectedAxis = current;
				picked = true;
			}
		}

		for ( int i = 0; i < 3; ++i )
		{
			AxisType current = AxisType_None;
			if ( i == 0 )
			{
				current = AxisType_RX;
			}
			else if ( i == 1 )
			{
				current = AxisType_RY;
			}
			else
			{
				current = AxisType_RZ;
			}

			if ( intersection::intersectRaySphere( m_pCamera->getMouseRay( ), m_aRotateBounds[ i ], intersection ) )
			{
				m_selectedAxis	= current;
				picked			= true;
			}
		}


		if ( !picked )
		{
			m_selectedAxis = AxisType_None;
		}
	}

	void TransformGizmo::render() const
	{
		//m_pRenderer->drawSphere( Vector3::forward, 3.0f, TIKI_COLOR_RED );

		debugrenderer::drawLineGrid( 2, 10, TIKI_COLOR_GRAY);

		for ( int i = 0; i < 6; ++i )
		{
			if ( m_selectedAxis == (AxisType)i )
			{
				debugrenderer::drawLineBox( m_aAxisBounds[ i ], TIKI_COLOR_GREEN);
			}
			else
			{
				debugrenderer::drawLineBox( m_aAxisBounds[ i ] );
			}
		}


		for ( int i = 0; i < 3; ++i )
		{
			AxisType current = AxisType_None;
			if ( i == 0 )
			{
				current = AxisType_RX;
			}
			else if ( i == 1 )
			{
				current = AxisType_RY;
			}
			else
			{
				current = AxisType_RZ;
			}

			if ( m_selectedAxis == current )
			{
				debugrenderer::drawLineSphere( m_aRotateBounds[ i ].center, m_aRotateBounds[ i ].radius, TIKI_COLOR_GREEN );
			}
			else
			{
				debugrenderer::drawLineSphere( m_aRotateBounds[ i ].center, m_aRotateBounds[ i ].radius );
			}
		}

		debugrenderer::drawSolidAxes( m_lineLength, m_lineOffset, m_worldMatrix);
	}
}