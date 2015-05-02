#pragma once

#ifndef TIKI_TRANSFORMGIZMO_HPP_INCLUDED__
#define TIKI_TRANSFORMGIZMO_HPP_INCLUDED__

namespace tiki
{

class	GraphicsSystem;
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
	
	void	create(GraphicsSystem* graphics);
	void	dispose();

	//void	OnKeyDown(KeyEventArgs^ e);
	//void	OnKeyUp(KeyEventArgs^ e);


	//property Vector3 Position
	//{
	//	Vector3 get();
	//	void    set(Vector3 value);
	//}

private:
	AxisType	m_AxisType;
	GizmoMode	m_GizmoMode;

	Vector3*	m_pPosition;
	Quaternion*	m_pRotation;

};



}

#endif // TIKI_TRANSFORMGIZMO_HPP_INCLUDED__
