
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

		m_pPosition				= nullptr;
		m_pRotation				= nullptr;
		m_pLastIntersection		= nullptr;
		m_pIntersection			= nullptr;
		m_pDelta				= nullptr;
		m_pTranslationDelta		= nullptr;

		m_pWorldMatrix			= nullptr;
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

		m_pLastIntersection = new Vector3();
		*m_pLastIntersection = Vector3::zero;

		m_pIntersection = new Vector3();
		*m_pIntersection = Vector3::zero;

		m_pDelta = new Vector3();
		*m_pDelta = Vector3::zero;

		m_pTranslationDelta = new Vector3();
		*m_pTranslationDelta = Vector3::zero;

		m_pWorldMatrix = new Matrix43();
		matrix::createIdentity( *m_pWorldMatrix );

		return true;
	}

	void TransformGizmo::dispose()
	{
		if ( m_pWorldMatrix )
		{
			delete m_pWorldMatrix;
			m_pWorldMatrix = nullptr;
		}

		if ( m_pTranslationDelta )
		{
			delete m_pTranslationDelta;
			m_pTranslationDelta = nullptr;
		}

		if ( m_pDelta )
		{
			delete m_pDelta;
			m_pDelta = nullptr;
		}

		if ( m_pIntersection )
		{
			delete m_pIntersection;
			m_pIntersection = nullptr;
		}

		if ( m_pLastIntersection )
		{
			delete m_pLastIntersection;
			m_pLastIntersection = nullptr;
		}

		if ( m_pRotation )
		{
			delete m_pRotation;
			m_pRotation = nullptr;
		}

		if ( m_pPosition )
		{
			delete m_pPosition;
			m_pPosition = nullptr;
		}

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

		if ( inputEvent.eventType == InputEventType_Keyboard_Down && inputEvent.data.keybaordKey.key == KeyboardKey_F1)
		{
			m_GizmoMode = GizmoMode::Translate;
		}

		if ( inputEvent.eventType == InputEventType_Keyboard_Up && inputEvent.data.keybaordKey.key == KeyboardKey_F1 )
		{
			m_GizmoMode = GizmoMode::None;
		}

		if ( inputEvent.eventType == InputEventType_Keyboard_Down && inputEvent.data.keybaordKey.key == KeyboardKey_F2 )
		{
			m_GizmoMode = GizmoMode::Scale;
		}

		if ( inputEvent.eventType == InputEventType_Keyboard_Up && inputEvent.data.keybaordKey.key == KeyboardKey_F2 )
		{
			m_GizmoMode = GizmoMode::None;
		}

		if ( inputEvent.eventType == InputEventType_Keyboard_Down && inputEvent.data.keybaordKey.key == KeyboardKey_F3 )
		{
			m_GizmoMode = GizmoMode::Rotate;
		}

		if ( inputEvent.eventType == InputEventType_Keyboard_Up && inputEvent.data.keybaordKey.key == KeyboardKey_F3 )
		{
			m_GizmoMode = GizmoMode::None;
		}

		return true;
	}

	void TransformGizmo::update(float timeDelta)
	{
		pickAxis();

		// save last intersection and reset 
		*m_pLastIntersection = *m_pIntersection;
		*m_pIntersection = Vector3::zero;
		*m_pTranslationDelta = Vector3::zero;


		if ( m_LeftMouseDown && m_SelectedAxis != AxisType::None )
		{
			Vector3 delta = Vector3::zero;

			if ( m_GizmoMode == GizmoMode::Translate ||
				 m_GizmoMode == GizmoMode::Scale)
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
							*m_pIntersection = intersection;

							if ( !vector::isEquals( *m_pLastIntersection, Vector3::zero) )
							{
								vector::set( *m_pDelta, m_pIntersection->x, m_pIntersection->y, m_pIntersection->z );
								vector::sub( *m_pDelta, *m_pLastIntersection );
							}

							if ( m_SelectedAxis == AxisType::X )
							{
								vector::set( delta, m_pDelta->x, 0.0f, 0.0f );
							}
							else
							{
								vector::set( delta, m_pDelta->x, m_pDelta->y, 0.0f );
							}
						}
					}
					break;
				case AxisType::Z:
				case AxisType::YZ:
				case AxisType::Y:
					{
						Plane plane;
						plane.create( Vector3::left, m_pPosition->x );

						Vector3 intersection;
						if ( intersection::intersectRayPlane( m_pCamera->getMouseRay(), plane, intersection ) )
						{
							*m_pIntersection = intersection;

							if ( !vector::isEquals( *m_pLastIntersection, Vector3::zero ) )
							{
								vector::set( *m_pDelta, m_pIntersection->x, m_pIntersection->y, m_pIntersection->z );
								vector::sub( *m_pDelta, *m_pLastIntersection );
							}

							if ( m_SelectedAxis == AxisType::Y )
							{
								vector::set( delta, 0.0f, m_pDelta->y, 0.0f );
							}
							else if ( m_SelectedAxis == AxisType::Z )
							{
								vector::set( delta, 0.0f, 0.0f, m_pDelta->z );
							}
							else
							{
								vector::set( delta, 0.0f, m_pDelta->y, m_pDelta->z );
							}
						}
					}
					break;
				case AxisType::XZ:
					{
						
						Plane plane;
						plane.create( Vector3::down, f32::abs(m_pPosition->y) ); // TODO: Anthony this is shit!

						Vector3 intersection;
						if ( intersection::intersectRayPlane( m_pCamera->getMouseRay(), plane, intersection ) )
						{
							*m_pIntersection = intersection;

							if ( !vector::isEquals( *m_pLastIntersection, Vector3::zero ) )
							{
								vector::set( *m_pDelta, m_pIntersection->x, m_pIntersection->y, m_pIntersection->z );
								vector::sub( *m_pDelta, *m_pLastIntersection );
							}
						}

						vector::set( delta, m_pDelta->x, 0.0f, m_pDelta->z );
					}
					break;
				case AxisType::None:
				case AxisType::Count:
				default:
					TIKI_ASSERT( false );
					break;
				}

				// vector::Transform(delta, rotation);

				if ( m_GizmoMode == GizmoMode::Translate )
				{
					
					*m_pTranslationDelta = delta;

					vector::add( *m_pPosition, *m_pTranslationDelta );

					m_pWorldMatrix->pos = *m_pPosition;

					if ( !vector::isEquals( *m_pTranslationDelta, Vector3::zero ) )
					{
						for ( int i = 0; i < AxisType::Count; ++i )
						{
							m_pAxisBounds[ i ].translate( *m_pTranslationDelta );
						}
					}

					// TODO: OnTransformGizmoTranslateEvent(delta)
				}
				else if ( m_GizmoMode == GizmoMode::Scale )
				{
					if ( !vector::isEquals( delta, Vector3::zero ) )
					{
						vector::add( delta, Vector3::one );

						for ( int i = 0; i < AxisType::Count; ++i )
						{
							vector::mul( m_pAxisBounds[ i ].Extents, delta );
						}
					}

					// TODO: OnTransformGizmoScaleEvent(delta)
				}

			} // Translate Gizmo mode

		}
	}

	void TransformGizmo::pickAxis()
	{
		// don't pick any axis if left mouse button is currently down or dragging the gizmo
		if ( m_LeftMouseDown )
		{
			return;
		}

		Vector3 intersection;
		bool picked = false;
		for ( int i = 0; i < AxisType::Count; ++i )
		{
			AxisType current = (AxisType)i;
			if ( intersection::intersectRayBox( m_pCamera->getMouseRay(), m_pAxisBounds[ current ], intersection ) )
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

		m_pRenderer->drawAxes( m_LineLength, m_LineOffset, *m_pWorldMatrix);
	}

}