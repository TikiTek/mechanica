#pragma once
#ifndef __TIKI_PHYSICSCOMPONENTS_INITDATA_HPP_INCLUDED__
#define __TIKI_PHYSICSCOMPONENTS_INITDATA_HPP_INCLUDED__

#include "tiki/base/reflection.hpp"
#include "tiki/base/structs.hpp"
#include "tiki/graphics/graphics_references.hpp"

namespace tiki
{
	enum PhysicsShapeType
	{
		ShapeType_Box,
		ShapeType_Capsule,
		ShapeType_Sphere
	};

	TIKI_REFLECTION_STRUCT(
		PhysicsShapeData,
		TIKI_REFLECTION_FIELD( PhysicsShapeType,	shapeType )
		TIKI_REFLECTION_FIELD( float3,				shapeBoxSize )
		TIKI_REFLECTION_FIELD( float,				shapeCapsuleRadius )
		TIKI_REFLECTION_FIELD( float,				shapeCapsuleHeight )
		TIKI_REFLECTION_FIELD( float,				shapeSphereRadius )
	);

	TIKI_REFLECTION_STRUCT(
		PhysicsBodyComponentInitData,
		TIKI_REFLECTION_FIELD( float3,				position )
		TIKI_REFLECTION_FIELD( float,				mass )
		TIKI_REFLECTION_FIELD( PhysicsShapeData,	shape )
	);

	TIKI_REFLECTION_STRUCT(
		PhysicsColliderComponentInitData,
		TIKI_REFLECTION_FIELD( float3,				position )
		TIKI_REFLECTION_FIELD( PhysicsShapeData,	shape )
	);
}

#endif // __TIKI_PHYSICSCOMPONENTS_INITDATA_HPP_INCLUDED__
