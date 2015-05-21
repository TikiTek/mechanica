
#include "tiki/math/intersection.hpp"
#include "tiki/base/float32.hpp"

namespace tiki 
{
	namespace intersection
	{
		bool intersectRaySpere( const Ray3& ray, const Sphere3& sphere, Vector3& intersectionPoint )
		{
			float rayParameters[ 2 ];

			Vector3 difference = ray.Origin; 
			vector::sub( difference, sphere.Center );

			float a0 = vector::dot( difference, difference ) - sphere.Radius * sphere.Radius;
			float a1, discr, root;

			// p is inside sphere
			if ( a0 <= 0.f )
			{
				a1					= vector::dot( ray.Direction, difference );
				discr				= a1 * a1 - a0;
				root				= f32::sqrt( discr );
				rayParameters[ 0 ]	= -a1 + root;

				Vector3 scaledDir = ray.Direction;
				vector::scale( scaledDir, rayParameters[ 0 ] );

				intersectionPoint = ray.Origin;
				vector::add( intersectionPoint, scaledDir );

				return true;
			}

			// P is outside the sphere
			a1 = vector::dot( ray.Direction, difference );
			if ( a1 >= 0.f )
			{
				return false;
			}

			discr = a1 * a1 - a0;
			if ( discr < 0.f )
			{
				return false;
			}
			// intersection type is a segment ( we have 2 intersection points, just return the first one for now)
			else if ( discr >= f32::zeroTolerance )
			{
				root = f32::sqrt( discr );
				rayParameters[ 0 ] = -a1 - root;
				rayParameters[ 1 ] = -a1 + root;

				Vector3 scaledDir = ray.Direction;
				vector::scale( scaledDir, rayParameters[ 0 ] );

				intersectionPoint = ray.Origin;
				intersectionPoint = vector::add( intersectionPoint, scaledDir );

				return true;
			}
			// intersection type is a point
			else
			{
				rayParameters[ 0 ] = -a1;

				Vector3 scaledDir = ray.Direction;
				vector::scale( scaledDir, rayParameters[ 0 ] );

				intersectionPoint = ray.Origin;
				intersectionPoint = vector::add( intersectionPoint, scaledDir );

				return true;
			}
		}

		// intersection = origin + t*direction, with t >= 0.
		bool intersectRayPlane( const Ray3& ray, const Plane& plane, Vector3& intersectionPoint )
		{
			Vector3 normal;
			plane.getNormal( normal );

			float dirDotNormal		= vector::dot( ray.Direction, normal );
			float signedDistance	= plane.getDistanceTo( ray.Origin );
			float lineParameter		= 0.f;

			// The line is not parallel to the plane, so they must intersect.
			if ( f32::abs( dirDotNormal ) > f32::zeroTolerance )
			{
				// get t
				lineParameter = -signedDistance / dirDotNormal;

				// t * direction;
				intersectionPoint = ray.Direction;
				vector::scale( intersectionPoint, lineParameter );

				// origin + t * direction;
				vector::add( intersectionPoint, ray.Origin );

				return true;
			}

			// The Line and plane are parallel.  Determine if they are numerically close enough to be coincident.
			if ( f32::abs( signedDistance <= f32::zeroTolerance ) )
			{
				// The line is coincident with the plane, so choose t = 0 for the parameter (initial value). 
				intersectionPoint = ray.Origin;
				return true;
			}

			return false;
		}

		bool intersectRayBox( const Ray3& ray, const Box& box, Vector3& intersectionPoint )
		{
			float WdU	[ 3 ];
			float AWdU	[ 3 ];
			float DdU	[ 3 ];
			float ADdU	[ 3 ];
			//float AWxDdu[ 3 ];
			//float RHS;

			Vector3 diff = ray.Origin;
			vector::sub( diff, box.m_Center );

			WdU	[ 0 ] = vector::dot( ray.Direction, box.m_Center );
			AWdU[ 0 ] = f32::abs( WdU[ 0 ] );
			DdU [ 0 ] = vector::dot( diff, box.m_Axis[ 0 ] );
			ADdU[ 0 ] = f32::abs( DdU[ 0 ] );

			if ( ADdU[ 0 ] > box.m_Extents.x && DdU[ 0 ] * WdU[ 0 ] >= 0.0f )
			{
				return false;
			}



			return true;

		}


	}
}