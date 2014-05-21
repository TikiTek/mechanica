#pragma once
#ifndef __TIKI_FREECAMERA_HPP_INCLUDED__
#define __TIKI_FREECAMERA_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	class Camera;
	struct Quaternion;
	struct InputEvent;

	class FreeCamera
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( FreeCamera );

	public:

		void		create( const Vector3& position, const Quaternion& rotation );
		void		dispose();

		bool		getMouseControl() const			{ return m_enableMouse; }
		void		setMouseControl( bool enabled ) { m_enableMouse = enabled; }

		void		update( Camera& targetCamera, float timeDelta );

		bool		processInputEvent( const InputEvent& inputEvent );

	private:

		float		m_speed;
		Vector3		m_position;
		Vector2		m_rotation;

		bool		m_enableMouse;

		Vector2		m_leftStickState;
		Vector2		m_rightStickState;
		float		m_leftTriggerState;
		float		m_rightTriggerState;

	};
}

#endif // __TIKI_FREECAMERA_HPP_INCLUDED__
