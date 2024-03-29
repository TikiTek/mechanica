
#include "tiki/math/projection.hpp"

namespace tiki
{
	void Projection::createPerspective( float width, float height, float fieldOfView, float nearPlane, float farPlane )
	{
		TIKI_ASSERT( fieldOfView > 0.0f && fieldOfView < f32::pi );
		TIKI_ASSERT( nearPlane > 0.0f );
		TIKI_ASSERT( farPlane > nearPlane );

		m_type			= ProjectionType_Perspective;
		m_nearPlane		= nearPlane;
		m_farPlane		= farPlane;
		m_width			= width;
		m_height		= height;
		m_fieldOfView	= fieldOfView;

		createMatrix();
	}

	void Projection::createOrthographic( float width, float height, float nearPlane, float farPlane )
	{
		TIKI_ASSERT( nearPlane >= 0.0f );
		TIKI_ASSERT( farPlane > nearPlane );

		m_type			= ProjectionType_Orthographic;
		m_nearPlane		= nearPlane;
		m_farPlane		= farPlane;
		m_width			= width;
		m_height		= height;

		createMatrix();
	}

	void Projection::createOrthographicCenter( float width, float height, float nearPlane, float farPlane )
	{
		TIKI_ASSERT( nearPlane >= 0.0f );
		TIKI_ASSERT( farPlane > nearPlane );

		m_type			= ProjectionType_OrthographicCenter;
		m_nearPlane		= nearPlane;
		m_farPlane		= farPlane;
		m_width			= width;
		m_height		= height;

		createMatrix();
	}

	Vector2 Projection::getClipPosition( const Vector2& screenPosition ) const
	{
		Vector2 clipPosition =
		{
			-1.0f + (screenPosition.x / m_width) * 2.0f,
			1.0f - (screenPosition.y / m_height) * 2.0f
		};
		vector::clamp( clipPosition, Vector2::negativeOne, Vector2::one );

		return clipPosition;
	}

	void Projection::createMatrix()
	{
		matrix::clear( m_matrix );

		switch ( m_type )
		{
		case ProjectionType_Orthographic:
			{
				const float depthRange = 1.0f / ( m_farPlane - m_nearPlane );

				m_matrix.x.x = 2.0f / m_width;
				m_matrix.y.y = 2.0f / m_height;
				m_matrix.z.z = depthRange;
				m_matrix.w.z = -m_nearPlane * depthRange;
				m_matrix.w.w = 1.0f;
			}
			break;

		case ProjectionType_OrthographicCenter:
			{
				const float depthRange = 1.0f / ( m_farPlane - m_nearPlane );

				m_matrix.x.x = 2.0f / m_width;
				m_matrix.y.y = 2.0f / m_height;
				m_matrix.z.z = depthRange;
				m_matrix.w.x = -1.0f;
				m_matrix.w.y = 1.0f;
				m_matrix.w.z = -m_nearPlane * depthRange;
				m_matrix.w.w = 1.0f;
			}
			break;

		case  ProjectionType_Perspective:
			{
				const float depthRange = m_farPlane / ( m_farPlane - m_nearPlane );
				const float aspectRatio = m_width / m_height;

				const float halfFov	= m_fieldOfView * 0.5f;
				const float yScale	= cosf( halfFov ) / sinf( halfFov );
				const float xScale	= aspectRatio * yScale;

				m_matrix.x.x = xScale;
				m_matrix.y.y = yScale;
				m_matrix.z.z = depthRange;
				m_matrix.z.w = 1.0f;
				m_matrix.w.z = -m_nearPlane * depthRange;
			}
			break;
		}
	}
}