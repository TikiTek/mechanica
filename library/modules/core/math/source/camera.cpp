
#include "tiki/math/camera.hpp"

#include "tiki/math/quaternion.hpp"
#include "tiki/base/numbers.hpp"

namespace tiki
{
	Camera::Camera()
	{
		m_isFrustumValid = false;
	}

	Camera::~Camera()
	{
	}

	void Camera::create( const Vector3& position, const Quaternion& rotation, const Projection* pProjection /*= nullptr*/, const Vector3& upVector /*= Vector3::unitY*/ )
	{
		m_upVector		= upVector;

		if ( pProjection != nullptr )
		{
			m_projection = *pProjection;
		}

		setTransform( position, rotation );

		m_isFrustumValid = false;
	}

	void Camera::setTransform( const Vector3& position, const Quaternion& rotation )
	{
		m_position = position;
		m_rotation = rotation;

		quaternion::toMatrix( m_world.rot, rotation );
		m_world.pos = position;

		createMatrix();
	}

	void Camera::createMatrix()
	{
		m_isFrustumValid = false;

		//Vector3 forward = Vector3::unitZ;
		//matrix::transform( forward, m_world.rot );

		Quaternion rot;
		quaternion::fromMatrix( rot, m_world.rot );

		Vector3 forward;
		quaternion::getForward( forward, rot );

		Vector3 zaxis = forward;
		vector::normalize( forward );

		Vector3 xaxis;
		vector::normalize( vector::cross( xaxis, m_upVector, zaxis ) );
		
		Vector3 yaxis;
		vector::normalize( vector::cross( yaxis, zaxis, xaxis ) );

		matrix::createIdentity( m_view );
		m_view.rot.x.x = xaxis.x;
		m_view.rot.x.y = yaxis.x;
		m_view.rot.x.z = zaxis.x;
		m_view.rot.y.x = xaxis.y;
		m_view.rot.y.y = yaxis.y;
		m_view.rot.y.z = zaxis.y;
		m_view.rot.z.x = xaxis.z;
		m_view.rot.z.y = yaxis.z;
		m_view.rot.z.z = zaxis.z;
		m_view.pos.x = -vector::dot( xaxis, m_world.pos );
		m_view.pos.y = -vector::dot( yaxis, m_world.pos );
		m_view.pos.z = -vector::dot( zaxis, m_world.pos );

		matrix::set( m_viewProjection, m_view );
		matrix::mul( m_viewProjection, m_projection.getMatrix() );
	}

	const Frustum& Camera::getFrustum()
	{
		if ( !m_isFrustumValid )
		{
			m_frustum.create( m_viewProjection );
			m_isFrustumValid = true;
		}

		return m_frustum;
	}

	void Camera::getCameraRay( sint16 mousePosX, sint16 mousePosY, int width, int height, Ray3* ray )
	{
		createMatrix();

		if ( ray == nullptr )
		{
			TIKI_ASSERT( "Invalid ray" );
			return;
		}

		float mouseX = (float)( mousePosX );
		float mouseY = (float)( mousePosY );

		Vector3 origin;
		vector::set( origin, mouseX, mouseY, 0.0f );
		matrix::unproject( origin, 0.0f, 0.0f, (float)width, (float)height, m_projection.getNearPlane(), m_projection.getFarPlane(), m_viewProjection );

		Vector3 direction;
		vector::set( direction, mouseX, mouseY, 1.0f );
		matrix::unproject( direction, 0.0f, 0.0f, (float)width, (float)height, m_projection.getNearPlane(), m_projection.getFarPlane(), m_viewProjection );

		vector::sub( direction, origin );
		vector::normalize( direction );

		ray->Origin		= origin;
		ray->Direction	= direction;
	}

}