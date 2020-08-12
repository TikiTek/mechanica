#pragma once
#ifndef TIKI_EDITORCAMERA_HPP_INCLUDED
#define TIKI_EDITORCAMERA_HPP_INCLUDED

#include "tiki\base\types.hpp"

#include "tiki/math/camera.hpp"
#include "tiki/runtimeshared/freecamera.hpp"
#include "tiki/math/ray.hpp"

namespace tiki
{
	class GraphicsSystem;
	struct InputEvent;

	class EditorCamera
	{
	public:

						EditorCamera();

		bool			create( GraphicsSystem& graphicsSystem );
		void			dispose();

		void			update( double timeDelta );

		void			resize( uint x, uint y );

		bool			processInputEvent( const InputEvent& inputEvent );

		const Camera&	getCamera() const { return m_camera; }
		const Ray&		getMouseRay() const { return m_ray; }

	private:

		Camera			m_camera;
		FreeCamera		m_controller;

		Vector2			m_mousePosition;

		Ray				m_ray;
	};
}

#endif // TIKI_EDITORCAMERA_HPP_INCLUDED
