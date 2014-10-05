#pragma once
#ifndef __TIKI_PHYSICSCOMPONENTS_SHARED_HPP_INCLUDED__
#define __TIKI_PHYSICSCOMPONENTS_SHARED_HPP_INCLUDED__

#include "tiki/components/physicscomponents_initdata.hpp"

#include "tiki/math/vector.hpp"

namespace tiki
{
	struct PhysicsComponentShape
	{
		PhysicsShapeType			shapeType;

		PhysicsBoxShape				boxShape;
		PhysicsCapsuleShape			capsuleShape;
		PhysicsSphereShape			sphereShape;
	};

	PhysicsShape* createPhysicsComponentShape( PhysicsComponentShape& shape, const PhysicsComponentShapeInitData& initData )
	{
		shape.shapeType	= initData.shapeType;

		PhysicsShape* pShape = nullptr;
		switch ( shape.shapeType )
		{
		case ShapeType_Box:
			shape.boxShape.create( vector::create( initData.shapeBoxSize ) );
			pShape = &shape.boxShape;
			break;

		case ShapeType_Capsule:
			shape.capsuleShape.create( initData.shapeCapsuleHeight, initData.shapeCapsuleRadius );
			pShape = &shape.capsuleShape;
			break;

		case ShapeType_Sphere:
			shape.sphereShape.create( initData.shapeSphereRadius );
			pShape = &shape.sphereShape;
			break;

		default:
			TIKI_TRACE_ERROR( "[PhysicsCharacterControllerComponent] Could not initilize State, because of unsupported ShapeType.\n" );
			break;
		}

		return pShape;
	}

	void disposePhysicsComponentShape( PhysicsComponentShape& shape )
	{
		switch ( shape.shapeType )
		{
		case ShapeType_Box:
			shape.boxShape.dispose();
			break;

		case ShapeType_Capsule:
			shape.capsuleShape.dispose();
			break;

		case ShapeType_Sphere:
			shape.sphereShape.dispose();
			break;

		default:
			TIKI_TRACE_ERROR( "[PhysicsCharacterControllerComponent] Could not dispose State, because of unsupported ShapeType.\n" );
			break;
		}
	}
}

#endif // __TIKI_PHYSICSCOMPONENTS_SHARED_HPP_INCLUDED__
