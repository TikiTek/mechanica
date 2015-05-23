#pragma once

#ifndef TIKI_TRANSFORMGIZMO_HPP_INCLUDED__
#define TIKI_TRANSFORMGIZMO_HPP_INCLUDED__

#include "tiki\math\quaternion.hpp"

namespace tiki
{

class	ImmediateRenderer;
struct	Quaternion;
struct	Vector3;

enum class AxisType
{
	X,
	Y,
	Z,

	XY,
	XZ,
	YZ,

	RX,
	RY,
	RZ,

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
	
	bool	create( ImmediateRenderer* renderer );
	void	dispose();

	void	render();

	//void	OnKeyDown(KeyEventArgs^ e);
	//void	OnKeyUp(KeyEventArgs^ e);


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
	AxisType			m_AxisType;
	GizmoMode			m_GizmoMode;

	Vector3*			m_pPosition;
	Quaternion*			m_pRotation;

	ImmediateRenderer*	m_pRenderer;
};



}

#endif // TIKI_TRANSFORMGIZMO_HPP_INCLUDED__
