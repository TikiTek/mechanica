
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

	void Camera::getCameraRay( Ray& ray, float mousePosX, float mousePosY, float width, float height ) const
	{
		vector::set( ray.origin, mousePosX, mousePosY, 0.0f );
		vector::set( ray.direction, mousePosX, mousePosY, 1.0f );

		matrix::unproject( ray.origin, 0.0f, 0.0f, width, height, m_projection.getNearPlane(), m_projection.getFarPlane(), m_viewProjection );
		matrix::unproject( ray.direction, 0.0f, 0.0f, width, height, m_projection.getNearPlane(), m_projection.getFarPlane(), m_viewProjection );

		vector::sub( ray.direction, ray.origin );
		vector::normalize( ray.direction );
	}
}