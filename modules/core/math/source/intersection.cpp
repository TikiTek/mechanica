
#include "tiki/math/intersection.hpp"
#include "tiki/base/float32.hpp"

namespace tiki 
{
	namespace intersection
	{
		bool IntersectRaySpere( const Ray3& ray, const Sphere3& sphere, Vector3& intersectionPoint )
		{
			float rayParameters[ 2 ];

			Vector3 difference = ray.Origin; 
			difference	= vector::sub( difference, sphere.Center );

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
	}

}