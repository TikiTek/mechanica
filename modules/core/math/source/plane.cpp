
#include "tiki/math/plane.hpp"

namespace tiki
{
	Plane::Plane()
	{
		vector::clear( m_data );
	}

	Plane::~Plane()
	{
	}

	void Plane::create( float a, float b, float c, float d )
	{
		vector::set( m_data, a, b, c, d );
	}

	void Plane::create( const Vector3& normal, float d )
	{
		vector::set( m_data, normal, d );
	}

	void Plane::create( const Vector3& point, const Vector3& normal )
	{
		vector::set( m_data, normal, -vector::dot( normal, point ) );
	}

	void Plane::create( const Vector3& point1, const Vector3& point2, const Vector3& point3 )
	{
		const float x1 = point2.x - point1.x;
		const float y1 = point2.y - point1.y;
		const float z1 = point2.z - point1.z;
		const float x2 = point3.x - point1.x;
		const float y2 = point3.y - point1.y;
		const float z2 = point3.z - point1.z;

		const float yz = (y1 * z2) - (z1 * y2);
		const float xz = (z1 * x2) - (x1 * z2);
		const float xy = (x1 * y2) - (y1 * x2);

		const float invPyth = f32::rsqrt( (yz * yz) + (xz * xz) + (xy * xy) );

		Vector3 normal = { yz, xz, xy };
		vector::scale( normal, invPyth );

		create( point1, normal );
	}

	void Plane::normalize()
	{
		vector::normalize( m_data );
	}

	void Plane::getNormal( Vector3& targetVector ) const
	{
		vector::set( targetVector, m_data.x, m_data.y, m_data.z );
	}
}