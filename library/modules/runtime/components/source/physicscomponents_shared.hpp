#pragma once
#ifndef __TIKI_PHYSICSCOMPONENTS_SHARED_HPP_INCLUDED__
#define __TIKI_PHYSICSCOMPONENTS_SHARED_HPP_INCLUDED__

#include "tiki/math/vector.hpp"

#include "components.hpp"

namespace tiki
{
	struct PhysicsComponentShape
	{
		PhysicsShapeType			shapeType;

		PhysicsBoxShape				boxShape;
		PhysicsCapsuleShape			capsuleShape;
		PhysicsSphereShape			sphereShape;
	};

	TIKI_INLINE PhysicsShape* createPhysicsComponentShape( PhysicsComponentShape& shape, const PhysicsComponentShapeInitData& initData )
	{
		shape.shapeType	= initData.type;

		PhysicsShape* pShape = nullptr;
		switch ( shape.shapeType )
		{
		case PhysicsShapeType_Box:
			shape.boxShape.create( vector::create( initData.boxSize ) );
			pShape = &shape.boxShape;
			break;

		case PhysicsShapeType_Capsule:
			shape.capsuleShape.create( initData.capsuleHeight, initData.capsuleRadius );
			pShape = &shape.capsuleShape;
			break;

		case PhysicsShapeType_Sphere:
			shape.sphereShape.create( initData.sphereRadius );
			pShape = &shape.sphereShape;
			break;

		default:
			TIKI_TRACE_ERROR( "[PhysicsCharacterControllerComponent] Could not initilize State, because of unsupported ShapeType.\n" );
			break;
		}

		return pShape;
	}

	TIKI_INLINE void disposePhysicsComponentShape( PhysicsComponentShape& shape )
	{
		switch ( shape.shapeType )
		{
		case PhysicsShapeType_Box:
			shape.boxShape.dispose();
			break;

		case PhysicsShapeType_Capsule:
			shape.capsuleShape.dispose();
			break;

		case PhysicsShapeType_Sphere:
			shape.sphereShape.dispose();
			break;

		default:
			TIKI_TRACE_ERROR( "[PhysicsCharacterControllerComponent] Could not dispose State, because of unsupported ShapeType.\n" );
			break;
		}
	}
}

#endif // __TIKI_PHYSICSCOMPONENTS_SHARED_HPP_INCLUDED__
