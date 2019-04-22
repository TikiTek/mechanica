#include "tiki/math/intersection.hpp"

#include "tiki/base/float32.hpp"
#include "tiki/base/functions.hpp"
#include "tiki/math/axis_aligned_box.hpp"
#include "tiki/math/box.hpp"
#include "tiki/math/line2.hpp"
#include "tiki/math/plane.hpp"
#include "tiki/math/polygon2.hpp"
#include "tiki/math/ray2.hpp"
#include "tiki/math/sphere.hpp"

namespace tiki
{
	namespace intersection
	{
		bool    doClipping( float t0, float t1, const Vector3& origin, const Vector3& direction, const Box& box, bool solid, int& quantity, Vector3& intersectionPoint );
		bool    clip( float denom, float numer, float& t0, float& t1 );
	}

	// 2d
	bool intersection::checkLineLineIntersection( const Line2& line1, const Line2& line2, Vector2& intersectionPoint )
	{
		const Vector2 s1 = line1.getDistance();
		const Vector2 s2 = line2.getDistance();

		const float s = (-s1.y * (line1.start.x - line2.start.x) + s1.x * (line1.start.y - line2.start.y)) / (-s2.x * s1.y + s1.x * s2.y);
		const float t = (s2.x * (line1.start.y - line2.start.y) - s2.y * (line1.start.x - line2.start.x)) / (-s2.x * s1.y + s1.x * s2.y);

		if( s >= 0.0f && s <= 1.0f && t >= 0.0f && t <= 1.0f )
		{
			intersectionPoint.x = line1.start.x + (t * s1.x);
			intersectionPoint.y = line1.start.y + (t * s1.y);
			return true;
		}

		return false;
	}

	// 3d
	bool intersection::intersectRaySphere( const Ray3& ray, const Sphere& sphere, Vector3& intersectionPoint )
	{
		float rayParameters[ 2 ];

		Vector3 difference = ray.origin;
		vector::sub( difference, sphere.center );

		float a0 = vector::dot( difference, difference ) - sphere.radius * sphere.radius;
		float a1, discr, root;

		// p is inside sphere
		if( a0 <= 0.f )
		{
			a1 = vector::dot( ray.direction, difference );
			discr = a1 * a1 - a0;
			root = f32::sqrt( discr );
			rayParameters[ 0 ] = -a1 + root;

			Vector3 scaledDir = ray.direction;
			vector::scale( scaledDir, rayParameters[ 0 ] );

			intersectionPoint = ray.origin;
			vector::add( intersectionPoint, scaledDir );

			return true;
		}

		// P is outside the sphere
		a1 = vector::dot( ray.direction, difference );
		if( a1 >= 0.f )
		{
			return false;
		}

		discr = a1 * a1 - a0;
		if( discr < 0.f )
		{
			return false;
		}
		// intersection type is a segment ( we have 2 intersection points, just return the first one for now)
		else if( discr >= f32::zeroTolerance )
		{
			root = f32::sqrt( discr );
			rayParameters[ 0 ] = -a1 - root;
			rayParameters[ 1 ] = -a1 + root;

			Vector3 scaledDir = ray.direction;
			vector::scale( scaledDir, rayParameters[ 0 ] );

			intersectionPoint = ray.origin;
			intersectionPoint = vector::add( intersectionPoint, scaledDir );

			return true;
		}
		// intersection type is a point
		else
		{
			rayParameters[ 0 ] = -a1;

			Vector3 scaledDir = ray.direction;
			vector::scale( scaledDir, rayParameters[ 0 ] );

			intersectionPoint = ray.origin;
			intersectionPoint = vector::add( intersectionPoint, scaledDir );

			return true;
		}
	}

	bool intersection::intersectRayPlane( const Ray3& ray, const Plane& plane, Vector3& intersectionPoint )
	{
		// intersection = origin + t*direction, with t >= 0.
		Vector3 normal;
		plane.getNormal( normal );

		float dirDotNormal = vector::dot( ray.direction, normal );
		float signedDistance = plane.getDistanceTo( ray.origin );

		// The line is not parallel to the plane, so they must intersect.
		if( f32::abs( dirDotNormal ) > f32::zeroTolerance )
		{
			// get t
			float lineParameter = -signedDistance / dirDotNormal;

			// t * direction;
			intersectionPoint = ray.direction;
			vector::scale( intersectionPoint, lineParameter );

			// origin + t * direction;
			vector::add( intersectionPoint, ray.origin );

			return true;
		}

		// The Line and plane are parallel.  Determine if they are numerically close enough to be coincident.
		if( f32::abs( signedDistance <= f32::zeroTolerance ) )
		{
			// The line is coincident with the plane, so choose t = 0 for the parameter (initial value).
			intersectionPoint = ray.origin;
			return true;
		}

		return false;
	}

	bool intersection::intersectRayAxisAlignedBox( const Ray3& ray, const AxisAlignedBox& box, Vector3& intersectionPoint )
	{
		float tmin = (box.min.x - ray.origin.x) / ray.direction.x;
		float tmax = (box.max.x - ray.origin.x) / ray.direction.x;

		if( tmin > tmax )
		{
			swap( tmin, tmax );
		}

		float tymin = (box.min.y - ray.origin.y) / ray.direction.y;
		float tymax = (box.max.y - ray.origin.y) / ray.direction.y;

		if( tymin > tymax )
		{
			swap( tymin, tymax );
		}

		if( (tmin > tymax) || (tymin > tmax) )
		{
			return false;
		}

		tmin = (tymin > tmin ? tymin : tmin);
		tmax = (tymax < tmax ? tymax : tmax);

		float tzmin = (box.min.z - ray.origin.z) / ray.direction.z;
		float tzmax = (box.max.z - ray.origin.z) / ray.direction.z;

		if( tzmin > tzmax )
		{
			swap( tzmin, tzmax );
		}

		if( (tmin > tzmax) || (tzmin > tmax) )
		{
			return false;
		}

		return true;
	}

	bool intersection::intersectRayBox( const Ray3& ray, const Box& box, Vector3& intersectionPoint )
	{
		float WdU[ 3 ];
		float AWdU[ 3 ];
		float DdU[ 3 ];
		float ADdU[ 3 ];
		float AWxDdU[ 3 ];
		float RHS;

		Vector3 diff = ray.origin;
		vector::sub( diff, box.center );

		WdU[ 0 ] = vector::dot( ray.direction, box.axis[ 0 ] );
		AWdU[ 0 ] = f32::abs( WdU[ 0 ] );
		DdU[ 0 ] = vector::dot( diff, box.axis[ 0 ] );
		ADdU[ 0 ] = f32::abs( DdU[ 0 ] );
		if( ADdU[ 0 ] > box.extents.x && DdU[ 0 ] * WdU[ 0 ] >= 0.0f )
		{
			return false;
		}

		WdU[ 1 ] = vector::dot( ray.direction, box.axis[ 1 ] );
		AWdU[ 1 ] = f32::abs( WdU[ 1 ] );
		DdU[ 1 ] = vector::dot( diff, box.axis[ 1 ] );
		ADdU[ 1 ] = f32::abs( DdU[ 1 ] );
		if( ADdU[ 1 ] > box.extents.y && DdU[ 1 ] * WdU[ 1 ] >= 0.0f )
		{
			return false;
		}

		WdU[ 2 ] = vector::dot( ray.direction, box.axis[ 2 ] );
		AWdU[ 2 ] = f32::abs( WdU[ 2 ] );
		DdU[ 2 ] = vector::dot( diff, box.axis[ 2 ] );
		ADdU[ 2 ] = f32::abs( DdU[ 2 ] );
		if( ADdU[ 2 ] > box.extents.z && DdU[ 2 ] * WdU[ 2 ] >= 0.0f )
		{
			return false;
		}

		Vector3 WxD;
		vector::cross( WxD, ray.direction, diff );

		AWxDdU[ 0 ] = f32::abs( vector::dot( WxD, box.axis[ 0 ] ) );
		RHS = box.extents.y * AWdU[ 2 ] + box.extents.z * AWdU[ 1 ];
		if( AWxDdU[ 0 ] > RHS )
		{
			return false;
		}

		AWxDdU[ 1 ] = f32::abs( vector::dot( WxD, box.axis[ 1 ] ) );
		RHS = box.extents.x * AWdU[ 2 ] + box.extents.z * AWdU[ 0 ];
		if( AWxDdU[ 1 ] > RHS )
		{
			return false;
		}

		AWxDdU[ 2 ] = f32::abs( vector::dot( WxD, box.axis[ 2 ] ) );
		RHS = box.extents.x * AWdU[ 1 ] + box.extents.y * AWdU[ 0 ];
		if( AWxDdU[ 2 ] > RHS )
		{
			return false;
		}

		// Get intersection point

		int quantity;
		return doClipping( 0.0f, NumberLimits<float32>::maxValue(), ray.origin, ray.direction, box, true, quantity, intersectionPoint );
	}

	IntersectionTypes intersection::intersectSphereAxisAlignedBox( const Sphere& sphere, const AxisAlignedBox& box )
	{
		const Vector3 nearestBoxPoint =
		{
			(sphere.center.x < box.min.x ? box.min.x - sphere.center.x : (sphere.center.x > box.max.x ? sphere.center.x - box.max.x : 0.0f)),
			(sphere.center.y < box.min.y ? box.min.y - sphere.center.y : (sphere.center.y > box.max.y ? sphere.center.y - box.max.y : 0.0f)),
			(sphere.center.z < box.min.z ? box.min.z - sphere.center.z : (sphere.center.z > box.max.z ? sphere.center.z - box.max.z : 0.0f)),
		};

		const Vector3 farthestBoxPoint =
		{
			TIKI_MAX( f32::abs( sphere.center.x - box.min.x ), f32::abs( sphere.center.x - box.max.x ) ),
			TIKI_MAX( f32::abs( sphere.center.y - box.min.y ), f32::abs( sphere.center.y - box.max.y ) ),
			TIKI_MAX( f32::abs( sphere.center.z - box.min.z ), f32::abs( sphere.center.z - box.max.z ) )
		};

		const float nearestDistance = vector::lengthSquared( nearestBoxPoint );
		const float farthestDistance = vector::lengthSquared( farthestBoxPoint );

		const float radiusSquared = sphere.radius * sphere.radius;
		if ( nearestDistance > radiusSquared )
		{
			return IntersectionTypes_Disjoint;
		}

		if( farthestDistance < radiusSquared )
		{
			return IntersectionTypes_Contains;
		}

		return IntersectionTypes_Intersects;
	}

	IntersectionTypes intersection::intersectSphereSphere( const Sphere& sphere1, const Sphere& sphere2 )
	{
		const float bothRadius = sphere1.radius + sphere2.radius;
		const float distanceSquared = vector::distanceSquared( sphere1.center, sphere2.center );
		if( distanceSquared > (bothRadius * bothRadius) )
		{
			return IntersectionTypes_Disjoint;
		}

		const float minRadius = TIKI_MIN( sphere1.radius, sphere2.radius );
		if( distanceSquared < (minRadius * minRadius) )
		{
			return IntersectionTypes_Contains;
		}

		return IntersectionTypes_Intersects;
	}

	bool intersection::doClipping( float t0, float t1, const Vector3& origin, const Vector3& direction, const Box& box, bool solid, int& quantity, Vector3& intersectionPoint )
	{
		// Convert linear component to box coordinates.
		Vector3 diff = origin;
		vector::sub( diff, origin );

		Vector3 BOrigin;
		vector::set( BOrigin,
			vector::dot( diff, box.axis[ 0 ] ),
			vector::dot( diff, box.axis[ 1 ] ),
			vector::dot( diff, box.axis[ 2 ] )
		);

		Vector3 BDirection;
		vector::set( BDirection,
			vector::dot( direction, box.axis[ 0 ] ),
			vector::dot( direction, box.axis[ 1 ] ),
			vector::dot( direction, box.axis[ 2 ] )
		);

		float saveT0 = t0, saveT1 = t1;
		bool notAllClipped = clip( +BDirection.x, -BOrigin.x - box.extents.x, t0, t1 ) &&
			clip( -BDirection.x, +BOrigin.x - box.extents.x, t0, t1 ) &&
			clip( +BDirection.y, -BOrigin.y - box.extents.y, t0, t1 ) &&
			clip( -BDirection.y, +BOrigin.y - box.extents.y, t0, t1 ) &&
			clip( +BDirection.z, -BOrigin.z - box.extents.z, t0, t1 ) &&
			clip( -BDirection.z, +BOrigin.z - box.extents.z, t0, t1 );

		bool ret = false;

		if( notAllClipped && (solid || t0 != saveT0 || t1 != saveT1) )
		{
			if( t1 > t0 )
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

	bool intersection::clip( float denom, float numer, float& t0, float& t1 )
	{
		// Return value is 'true' if line segment intersects the current test
		// plane.  Otherwise 'false' is returned in which case the line segment
		// is entirely clipped.

		if( denom > 0.0f )
		{
			if( numer > denom*t1 )
			{
				return false;
			}
			if( numer > denom*t0 )
			{
				t0 = numer / denom;
			}
			return true;
		}
		else if( denom < 0.0f )
		{
			if( numer > denom*t0 )
			{
				return false;
			}
			if( numer > denom*t1 )
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
}