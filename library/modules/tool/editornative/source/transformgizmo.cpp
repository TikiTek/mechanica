
#include "tiki/editornative/transformgizmo.hpp"

#include "tiki/math/vector.hpp"
#include "tiki/math/quaternion.hpp"
#include "tiki/graphics/immediaterenderer.hpp"

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

	bool TransformGizmo::create(ImmediateRenderer* renderer )
	{
		m_pRenderer = renderer;

		return true;
	}

	void TransformGizmo::dispose()
	{

	}

	void TransformGizmo::render()
	{
		m_pRenderer->drawGrid();
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