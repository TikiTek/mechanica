#pragma once
#ifndef TIKI_INTERSECTION_HPP__
#define TIKI_INTERSECTION_HPP__

#include "tiki/math/vector.hpp"

namespace tiki
{
	namespace intersection
	{
#pragma region IntersectionObjects
		struct Ray3
		{
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
			Vector3 Center;
			float	Radius;

			Sphere3( const Vector3& center, float radius )
			{
				Center = center;
				Radius = radius;
			}
		};
#pragma endregion

		bool	IntersectRaySpere( const Ray3& ray, const Sphere3& sphere, Vector3& intersectionPoint);



	} // namespace intersection
} // namespace tiki

#endif // TIKI_INTERSECTION_HPP__
