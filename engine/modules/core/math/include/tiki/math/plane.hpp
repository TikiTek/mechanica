#pragma once
#ifndef __TIKI_PLANE_HPP_INCLUDED__
#define __TIKI_PLANE_HPP_INCLUDED__

#include "tiki/math/vector.hpp"

namespace tiki
{
	enum PlaneIntersectionType
	{
		PlaneIntersectionType_Back,
		PlaneIntersectionType_Front,
		PlaneIntersectionType_Intersecting
	};

	class Plane
	{
	public:

						Plane();
						~Plane();

		void			create( float a, float b, float c, float d );
		void			create( const Vector3& normal, float d );
		void			create( const Vector3& point, const Vector3& normal );
		void			create( const Vector3& point1, const Vector3& point2, const Vector3& point3 );

		void			normalize();

		void			getNormal( Vector3& targetVector ) const;
		float			getD() const { return m_data.w; }
		const Vector4&	getData() const { return m_data; }

		float			getDistanceTo( const Vector3& target ) const;

	private:

		Vector4		m_data;

	};
}

#endif // __TIKI_PLANE_HPP_INCLUDED__
