
#include "tiki/math/camera.hpp"

#include "tiki/base/numbers.hpp"
#include "tiki/math/quaternion.hpp"
#include "tiki/math/ray3.hpp"

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

		quaternion::toMatrix( m_world, rotation );
		
		Matrix44 translationMatrix;
		matrix::createTranslation( translationMatrix, position );
		matrix::mul( m_world, translationMatrix );

		createData();
	}

	void Camera::createData()
	{
		matrix::invert( m_view, m_world );
		matrix::mul( m_viewProjection, m_view, m_projection.getMatrix() );

		m_frustum.create( m_viewProjection );
	}

	const Frustum& Camera::getFrustum() const
	{
		return m_frustum;
	}

	void Camera::getCameraRay( Ray3& ray, const Vector2& screenPosition ) const
	{
		const Vector2 clipPosition = m_projection.getClipPosition( screenPosition );

		Vector3 target;
		vector::set( ray.origin, clipPosition, -1.0f );
		vector::set( target, clipPosition, 1.0f );

		Matrix44 inverseViewProjection;
		matrix::invert( inverseViewProjection, m_viewProjection );

		matrix::transform( ray.origin, inverseViewProjection );
		matrix::transform( target, inverseViewProjection );

		vector::sub( ray.direction, target, ray.origin );
		vector::normalize( ray.direction );
	}
}