
#include "tiki/editornative/transformgizmo.hpp"

#include "tiki/math/vector.hpp"
#include "tiki/math/quaternion.hpp"

namespace tiki
{
	TransformGizmo::TransformGizmo()
		: m_GizmoMode(GizmoMode::None),
		  m_AxisType(AxisType::None)
	{
	}

	TransformGizmo::~TransformGizmo()
	{

	}

	void TransformGizmo::create(GraphicsSystem* graphics)
	{
	}

	void TransformGizmo::dispose()
	{

	}

#pragma region Properties

	//Vector3 TransformGizmo::Position::get()
	//{
	//	return *m_pPosition;
	//}
	//
	//void TransformGizmo::Position::set(Vector3 value)
	//{
	//	*m_pPosition = value;
	//}

#pragma endregion



}