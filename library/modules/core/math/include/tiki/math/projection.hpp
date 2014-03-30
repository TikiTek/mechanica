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

		void			createPerspective( float aspectRatio, float fieldOfView, float nearPlane, float farPlane );
		void			createOrthographic( float width, float height, float nearPlane, float farPlane );
		void			createOrthographicCenter( float width, float height, float nearPlane, float farPlane );

		const Matrix44&	getMatrix() const { return m_matrix; }

	private:

		Matrix44		m_matrix;

		ProjectionType	m_type;
		float			m_nearPlane;
		float			m_farPlane;

		union 
		{
			struct
			{
				float			width;
				float			height;
			} m_orthographicData;
			struct
			{
				float			aspectRatio;
				float			fieldOfView;
			} m_perspectiveData;
		};

		void			createMatrix();

	};
}

#endif // __TIKI_PROJECTION_HPP_INCLUDED__
