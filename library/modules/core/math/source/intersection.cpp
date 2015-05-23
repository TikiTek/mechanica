
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

		bool intersectRayPlane( const Ray3& ray, const Plane& plane, Vector3& intersectionPoint )
		{
			// intersection = origin + t*direction, with t >= 0.
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

		#pragma region intersectRayBox
		bool intersectRayBox( const Ray3& ray, const Box& box, Vector3& intersectionPoint )
		{
			float WdU	[ 3 ];
			float AWdU	[ 3 ];
			float DdU	[ 3 ];
			float ADdU	[ 3 ];
			float AWxDdU[ 3 ];
			float RHS;

			Vector3 diff = ray.Origin;
			vector::sub( diff, box.Center );

			WdU	[ 0 ] = vector::dot( ray.Direction, box.Axis[ 0 ] );
			AWdU[ 0 ] = f32::abs( WdU[ 0 ] );
			DdU [ 0 ] = vector::dot( diff, box.Axis[ 0 ] );
			ADdU[ 0 ] = f32::abs( DdU[ 0 ] );
			if ( ADdU[ 0 ] > box.Extents.x && DdU[ 0 ] * WdU[ 0 ] >= 0.0f )
			{
				return false;
			}

			WdU	[ 1 ] = vector::dot( ray.Direction, box.Axis[ 1 ] );
			AWdU[ 1 ] = f32::abs( WdU[ 1 ] );
			DdU	[ 1 ] = vector::dot( diff, box.Axis[ 1 ] );
			ADdU[ 1 ] = f32::abs( DdU[ 1 ] );
			if ( ADdU[ 1 ] > box.Extents.y && DdU[ 1 ] * WdU[ 1 ] >= 0.0f )
			{
				return false;
			}

			WdU	[ 2 ] = vector::dot( ray.Direction, box.Axis[ 2 ] );
			AWdU[ 2 ] = f32::abs( WdU[ 2 ] );
			DdU	[ 2 ] = vector::dot( diff, box.Axis[ 2 ] );
			ADdU[ 2 ] = f32::abs( DdU[ 2 ] );
			if ( ADdU[ 2 ] > box.Extents.z && DdU[ 2 ] * WdU[ 2 ] >= 0.0f )
			{
				return false;
			}

			Vector3 WxD;
			vector::cross( WxD, ray.Direction, diff );

			AWxDdU[ 0 ] = f32::abs( vector::dot( WxD, box.Axis[ 0 ] ) ); 
			RHS = box.Extents.y * AWdU[ 2 ] + box.Extents.z * AWdU[ 1 ];
			if ( AWxDdU[ 0 ] > RHS )
			{
				return false;
			}

			AWxDdU[ 1 ] = f32::abs( vector::dot( WxD, box.Axis[ 1 ] ) ); 
			RHS = box.Extents.x * AWdU[ 2 ] + box.Extents.z * AWdU[ 0 ];
			if ( AWxDdU[ 1 ] > RHS )
			{
				return false;
			}

			AWxDdU[ 2 ] = f32::abs( vector::dot( WxD, box.Axis[ 2 ] ) );
			RHS = box.Extents.x * AWdU[ 1 ] + box.Extents.y * AWdU[ 0 ];
			if ( AWxDdU[ 2 ] > RHS )
			{
				return false;
			}

			// Get intersection point

			int quantity;
			return doClipping( 0.0f, f32::maxValue, ray.Origin, ray.Direction, box, true, quantity, intersectionPoint );

		}

		bool doClipping( float t0, float t1, const Vector3& origin, const Vector3& direction, const Box& box, bool solid, int& quantity, Vector3& intersectionPoint )
		{
			// Convert linear component to box coordinates.
			Vector3 diff = origin;
			vector::sub( diff, origin );

			Vector3 BOrigin;
			vector::set( BOrigin,
				vector::dot( diff, box.Axis[ 0 ] ),
				vector::dot( diff, box.Axis[ 1 ] ),
				vector::dot( diff, box.Axis[ 2 ] ) );

			Vector3 BDirection;
			vector::set( BDirection,
				vector::dot( direction, box.Axis[ 0 ] ),
				vector::dot( direction, box.Axis[ 1 ] ),
				vector::dot( direction, box.Axis[ 2 ] ) );

			float saveT0 = t0, saveT1 = t1;
			bool notAllClipped = clip( +BDirection.x, -BOrigin.x - box.Extents.x, t0, t1 ) &&
				clip( -BDirection.x, +BOrigin.x - box.Extents.x, t0, t1 ) &&
				clip( +BDirection.y, -BOrigin.y - box.Extents.y, t0, t1 ) &&
				clip( -BDirection.y, +BOrigin.y - box.Extents.y, t0, t1 ) &&
				clip( +BDirection.z, -BOrigin.z - box.Extents.z, t0, t1 ) &&
				clip( -BDirection.z, +BOrigin.z - box.Extents.z, t0, t1 );

			bool ret = false;

			if ( notAllClipped && (solid || t0 != saveT0 || t1 != saveT1) )
			{
				if ( t1 > t0 )
				{
					// intersection type is a segment
					ret = true;
					quantity = 2;

					Vector3 scaledDir;
					vector::scale( scaledDir, t0 );
					intersectionPoint = origin;
					vector::add( intersectionPoint, scaledDir );

					// NOTE: second intersectionPoint: point[ 1 ] = origin + t1*direction;
				}
				else
				{
					// intersection type is a point
					ret = true;
					quantity = 1;

					Vector3 scaledDir;
					vector::scale( scaledDir, t0 );
					intersectionPoint = origin;
					vector::add( intersectionPoint, scaledDir );
				}
			}

			return ret;
		}

		bool clip( float denom, float numer, float& t0, float& t1 )
		{
			// Return value is 'true' if line segment intersects the current test
			// plane.  Otherwise 'false' is returned in which case the line segment
			// is entirely clipped.

			if ( denom > 0.0f )
			{
				if ( numer > denom*t1 )
				{
					return false;
				}
				if ( numer > denom*t0 )
				{
					t0 = numer / denom;
				}
				return true;
			}
			else if ( denom < 0.0f )
			{
				if ( numer > denom*t0 )
				{
					return false;
				}
				if ( numer > denom*t1 )
				{
					t1 = numer / denom;
				}
				return true;
			}
			else
			{
				return numer <= 0.0f;
			}
		}
		#pragma endregion


	}
}