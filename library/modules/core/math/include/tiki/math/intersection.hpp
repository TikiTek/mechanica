#pragma once
#ifndef TIKI_INTERSECTION_HPP__
#define TIKI_INTERSECTION_HPP__

#include "tiki/math/vector.hpp"

namespace tiki
{
	class Plane;
	struct Box;
	struct Ray;
	struct Sphere;

	namespace intersection
	{
		bool	intersectRaySphere( const Ray& ray, const Sphere& sphere, Vector3& intersectionPoint );
		bool	intersectRayPlane( const Ray& ray, const Plane& plane, Vector3& intersectionPoint );

		bool    intersectRayBox( const Ray& ray, const Box& box, Vector3& intersectionPoint);
		bool    doClipping( float t0, float t1, const Vector3& origin, const Vector3& direction, const Box& box, bool solid, int& quantity, Vector3& intersectionPoint);
		bool    clip( float denom, float numer, float& t0, float& t1 );

	}
}

#endif // TIKI_INTERSECTION_HPP__
