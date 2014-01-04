
#include "tiki/math/camera.hpp"

#include "tiki/math/quaternion.hpp"

namespace tiki
{
	void Camera::create( const Vector3& position, const Quaternion& rotation, const Projection& projection, const Vector3& upVector /*= Vector3::unitY*/ )
	{
		m_upVector		= upVector;
		m_projection	= projection;
		setTransform( position, rotation );
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

}