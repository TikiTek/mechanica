#pragma once
#ifndef TIKI_PHYSICS3DCOMPONENTS_SHARED_HPP_INCLUDED
#define TIKI_PHYSICS3DCOMPONENTS_SHARED_HPP_INCLUDED

#include "tiki/math/vector.hpp"

#include "components.hpp"

namespace tiki
{
	struct Physics3dComponentShape
	{
		Physics3dShapeType			shapeType;

		Physics3dBoxShape			boxShape;
		Physics3dCapsuleShape		capsuleShape;
		Physics3dSphereShape		sphereShape;
	};

	TIKI_INLINE Physics3dShape* createPhysics3dComponentShape( Physics3dComponentShape& shape, const Physics3dComponentShapeInitData& initData )
	{
		shape.shapeType	= initData.type;

		Physics3dShape* pShape = nullptr;
		switch ( shape.shapeType )
		{
		case Physics3dShapeType_Box:
			shape.boxShape.create( vector::create( initData.boxSize ) );
			pShape = &shape.boxShape;
			break;

		case Physics3dShapeType_Capsule:
			shape.capsuleShape.create( initData.capsuleHeight, initData.capsuleRadius );
			pShape = &shape.capsuleShape;
			break;

		case Physics3dShapeType_Sphere:
			shape.sphereShape.create( initData.sphereRadius );
			pShape = &shape.sphereShape;
			break;

		default:
			TIKI_TRACE_ERROR( "[PhysicsCharacterControllerComponent] Could not initilize State, because of unsupported ShapeType.\n" );
			break;
		}

		return pShape;
	}

	TIKI_INLINE void disposePhysics3dComponentShape( Physics3dComponentShape& shape )
	{
		switch ( shape.shapeType )
		{
		case Physics3dShapeType_Box:
			shape.boxShape.dispose();
			break;

		case Physics3dShapeType_Capsule:
			shape.capsuleShape.dispose();
			break;

		case Physics3dShapeType_Sphere:
			shape.sphereShape.dispose();
			break;

		default:
			TIKI_TRACE_ERROR( "[PhysicsCharacterControllerComponent] Could not dispose State, because of unsupported ShapeType.\n" );
			break;
		}
	}
}

#endif // TIKI_PHYSICS3DCOMPONENTS_SHARED_HPP_INCLUDED
