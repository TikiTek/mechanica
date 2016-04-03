#pragma once
#ifndef TIKI_INTERSECTION_HPP__
#define TIKI_INTERSECTION_HPP__

#include "tiki/base/types.hpp"

namespace tiki
{
	class Plane;
	struct AxisAlignedBox;
	struct Box;
	struct Ray;
	struct Sphere;
	struct Vector3;

	enum IntersectionTypes
	{
		IntersectionTypes_Disjoint,
		IntersectionTypes_Contains,
		IntersectionTypes_Intersects
	};

	namespace intersection
	{
		bool				intersectRaySphere( const Ray& ray, const Sphere& sphere, Vector3& intersectionPoint );
		bool				intersectRayPlane( const Ray& ray, const Plane& plane, Vector3& intersectionPoint );
		bool				intersectRayAxisAlignedBox( const Ray& ray, const AxisAlignedBox& box, Vector3& intersectionPoint );
		bool				intersectRayBox( const Ray& ray, const Box& box, Vector3& intersectionPoint );

		IntersectionTypes   intersectSphereAxisAlignedBox( const Sphere& sphere, const AxisAlignedBox& box );
		IntersectionTypes   intersectSphereSphere( const Sphere& sphere1, const Sphere& sphere2 );
	}
}

#endif // TIKI_INTERSECTION_HPP__
