
#include "tiki/math/camera.hpp"

#include "tiki/base/numbers.hpp"
#include "tiki/math/quaternion.hpp"
#include "tiki/math/ray.hpp"

namespace tiki
{
	Camera::Camera()
	{
	}

	Camera::~Camera()
	{
	}

	void Camera::create( const Camera& copy )
	{
		m_projection	= copy.m_projection;

		setTransform( copy.m_position, copy.m_rotation );
	}

	void Camera::create( const Vector3& position /* = Vector3::zero */, const Quaternion& rotation /* = Quaternion::identity */, const Projection* pProjection /* = nullptr */ )
	{
		if ( pProjection != nullptr )
		{
			m_projection = *pProjection;
		}

		setTransform( position, rotation );
	}

	void Camera::setProjection( const Projection& projection )
	{
		m_projection = projection;
		createData();
	}

	void Camera::setTransform( const Vector3& position, const Quaternion& rotation )
	{
		m_position = position;
		m_rotation = rotation;

		Matrix33 rotationMatrix;
		quaternion::toMatrix( rotationMatrix, rotation );
				
		matrix::set( m_world, rotationMatrix );

		Matrix44 translationMatrix;
		matrix::createTranslation( translationMatrix, position );
		matrix::mul( m_world, translationMatrix );

		createData();
	}

	void Camera::createData()
	{
		matrix::invert( m_view, m_world );

		m_viewProjection = m_view;
		matrix::mul( m_viewProjection, m_projection.getMatrix() );

		m_frustum.create( m_viewProjection );
	}

	const Frustum& Camera::getFrustum() const
	{
		return m_frustum;
	}

	void Camera::getCameraRay( Ray& ray, const Vector2& screenPosition ) const
	{
		Vector2 clipPosition = 
		{
			-1.0f + (screenPosition.x / m_projection.getWidth()) * 2.0f,
			 1.0f - (screenPosition.y / m_projection.getHeight()) * 2.0f
		};
		vector::clamp( clipPosition, Vector2::negativeOne, Vector2::one );
		//vector::div( clipPosition, screenPosition, vector::create( m_projection.getWidth(), m_projection.getHeight() ) );
		//vector::mul( clipPosition, vector::create( 0.5f, -0.5f ) );
		//vector::add( clipPosition, vector::create( 0.5f, -0.5f ) );

		vector::set( ray.origin, clipPosition, -1.0f );
		vector::set( ray.direction, clipPosition, 1.0f );

		Matrix44 inverseViewProjection;
		matrix::invert( inverseViewProjection, m_viewProjection );

		matrix::transform( ray.origin, inverseViewProjection );
		matrix::transform( ray.direction, inverseViewProjection );

		vector::sub( ray.direction, ray.origin );
		vector::normalize( ray.direction );
	}
}