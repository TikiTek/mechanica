#pragma once
#ifndef TIKI_TRANSFORMGIZMO_HPP_INCLUDED__
#define TIKI_TRANSFORMGIZMO_HPP_INCLUDED__

#include "tiki\math\box.hpp"
#include "tiki\math\quaternion.hpp"
#include "tiki\math\sphere.hpp"

namespace tiki
{
	class ImmediateRenderer;
	class EditorCamera;
	struct InputEvent;

	enum AxisType
	{
		AxisType_X,
		AxisType_Y,
		AxisType_Z,
		AxisType_XZ,
		AxisType_XY,
		AxisType_YZ,	
		AxisType_RX,
		AxisType_RY,
		AxisType_RZ,
	
		AxisType_Count,
		AxisType_None
	};

	enum GizmoMode
	{
		GizmoMode_Translate,
		GizmoMode_Rotate, 
		GizmoMode_Scale,

		GizmoMode_Count,
		GizmoMode_None
	};

	class TransformGizmo
	{
	public:

							TransformGizmo();
							~TransformGizmo();
	
		bool				create( const EditorCamera& camera );
		void				dispose();


		bool				processInputEvent( const InputEvent& inputEvent );

		void				update( double timeDelta );
		void				pickAxis();

		void				render() const;

		const Vector3&		getPosition() const { return m_position; }
		void				setPosition( const Vector3& position ) { m_position = position; }

		const Quaternion&	getRotation() const { return m_rotation; }
		void				setRotation( const Quaternion& rotation ) { m_rotation = rotation; }

	private:

		AxisType			m_selectedAxis;
		GizmoMode			m_gizmoMode;

		Vector3				m_position;
		Quaternion			m_rotation;

		float				m_lineOffset;
		float				m_lineLength;
		float				m_multiAxisThickness;
		float				m_singleAxisThickness;

		const EditorCamera*	m_pCamera;

		// TODO: GizmoAxis
		Box					m_aAxisBounds[ 6u ];
		Sphere				m_aRotateBounds[ 3u ];

		bool				m_leftMouseDown;

		Vector3				m_lastIntersection;
		Vector3				m_intersection;
		Vector3				m_delta;
		Vector3				m_translationDelta;

		Matrix43			m_worldMatrix;

		float				m_mouseOffsetX;
	};
}

#endif // TIKI_TRANSFORMGIZMO_HPP_INCLUDED__
