#pragma once
#ifndef __TIKI_PROJECTION_HPP_INCLUDED__
#define __TIKI_PROJECTION_HPP_INCLUDED__

#include "tiki/math/matrix.hpp"

namespace tiki
{
	enum ProjectionType
	{
		ProjectionType_Invalid,

		ProjectionType_Orthographic,
		ProjectionType_OrthographicCenter,
		ProjectionType_Perspective
	};

	class Projection
	{
	public:

		void			createPerspective( float width, float height, float fieldOfView, float nearPlane, float farPlane );
		void			createOrthographic( float width, float height, float nearPlane, float farPlane );
		void			createOrthographicCenter( float width, float height, float nearPlane, float farPlane );

		const Matrix44&	getMatrix() const		{ return m_matrix; }

		float			getNearPlane() const	{ return m_nearPlane; }
		float			getFarPlane() const		{ return m_farPlane; }
		float			getWidth() const		{ return m_width; }
		float			getHeight() const		{ return m_height; }
		float			getFieldOfView() const	{ return m_fieldOfView; }

	private:

		Matrix44		m_matrix;

		ProjectionType	m_type;
		float			m_nearPlane;
		float			m_farPlane;
		float			m_width;
		float			m_height;
		float			m_fieldOfView;

		void			createMatrix();

	};
}

#endif // __TIKI_PROJECTION_HPP_INCLUDED__
