#pragma once

#ifndef TIKI_TRANSFORMGIZMO_HPP_INCLUDED__
#define TIKI_TRANSFORMGIZMO_HPP_INCLUDED__

#include "tiki\math\quaternion.hpp"

namespace tiki
{

class	ImmediateRenderer;
struct	Quaternion;
struct	Vector3;
struct  InputEvent;
struct  Box;

ref class EditorCamera;

enum AxisType
{
	X,
	Y,
	Z,

	XZ,
	XY,
	YZ,
	//
	//RX,
	//RY,
	//RZ,
	//
	Count,

	None
};

enum class GizmoMode
{
	Translate,
	Rotate, 
	Scale,

	None
};

public ref class TransformGizmo
{
public:
			TransformGizmo();
			~TransformGizmo();
	
	bool	create( ImmediateRenderer* renderer, EditorCamera^ camera );
	void	dispose();


	bool	processInputEvent( InputEvent& inputEvent );

	void	update( float timeDelta );
	void	pickAxis();

	void	render();


	property Vector3 Position
	{
		Vector3 get() { return *m_pPosition; }
		void    set( Vector3 value ) { *m_pPosition = value; }
	}

	property Quaternion Rotation
	{
		Quaternion get( ) { return *m_pRotation; }
		void       set( Quaternion value )  { *m_pRotation = value; }
	}

private:
	AxisType			m_SelectedAxis;
	GizmoMode			m_GizmoMode;

	Vector3*			m_pPosition;
	Quaternion*			m_pRotation;

	float				m_LineOffset;
	float				m_LineLength;
	float				m_MultiAxisThickness;
	float				m_SingleAxisThickness;

	ImmediateRenderer*	m_pRenderer;
	EditorCamera^		m_pCamera;

	Box*				m_pAxisBounds;

	bool				m_LeftMouseDown;

	Vector3*			m_pLastIntersection;
	Vector3*			m_pIntersection;
	Vector3*			m_pDelta;
	Vector3*			m_pTranslationDelta;

	Matrix43*			m_pWorldMatrix;
};



}

#endif // TIKI_TRANSFORMGIZMO_HPP_INCLUDED__
