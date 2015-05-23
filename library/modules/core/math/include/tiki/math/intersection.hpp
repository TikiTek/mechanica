#pragma once
#ifndef TIKI_INTERSECTION_HPP__
#define TIKI_INTERSECTION_HPP__

#include "tiki/math/vector.hpp"
#include "tiki/math/plane.hpp"
#include "tiki/math/box.hpp"

namespace tiki
{
	namespace intersection
	{
#pragma region IntersectionObjects
		struct Ray3
		{
			TIKI_NONCOPYABLE_STRUCT( Ray3 );

			Vector3 Origin;
			Vector3 Direction;

			Ray3( const Vector3& origin, const Vector3& direction )
			{
				Origin = origin;
				Direction = direction;
			}
		};

		struct Sphere3
		{
			TIKI_NONCOPYABLE_STRUCT( Sphere3 );

			Vector3 Center;
			float	Radius;

			Sphere3( const Vector3& center, float radius )
			{
				Center = center;
				Radius = radius;
			}
		};
#pragma endregion

		bool	intersectRaySpere( const Ray3& ray, const Sphere3& sphere, Vector3& intersectionPoint );
		bool	intersectRayPlane( const Ray3& ray, const Plane& plane, Vector3& intersectionPoint );

		bool    intersectRayBox( const Ray3& ray, const Box& box, Vector3& intersectionPoint);
		bool    doClipping( float t0, float t1, const Vector3& origin, const Vector3& direction, const Box& box, bool solid, int& quantity, Vector3& intersectionPoint);
		bool    clip( float denom, float numer, float& t0, float& t1 );

	} // namespace intersection
} // namespace tiki

#endif // TIKI_INTERSECTION_HPP__
