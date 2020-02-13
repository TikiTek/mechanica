#pragma once

#include "tiki/math/vector.hpp"
#include "tiki/physics_2d/physics_2d_box_shape.hpp"
#include "tiki/physics_2d/physics_2d_circle_shape.hpp"
#include "tiki/physics_2d/physics_2d_polygon_shape.hpp"

#include "physics_2d_components.hpp"

namespace tiki
{
	struct Physics2dComponentShape
	{
		Physics2dShapeType			shapeType;

		Physics2dBoxShape			boxShape;
		Physics2dCircleShape		circleShape;
		Physics2dPolygonShape		polygonShape;
	};

	TIKI_INLINE Physics2dShape* createPhysics2dComponentShape( Physics2dComponentShape& shape, const Physics2dComponentShapeInitData& initData )
	{
		shape.shapeType	= initData.type;

		Physics2dShape* pShape = nullptr;
		switch ( shape.shapeType )
		{
		case Physics2dShapeType_Box:
			shape.boxShape.create( vector::create( initData.boxSize ) );
			pShape = &shape.boxShape;
			break;

		case Physics2dShapeType_Circle:
			shape.circleShape.create( initData.circleRadius );
			pShape = &shape.circleShape;
			break;

		case Physics2dShapeType_Polygon:
			TIKI_ASSERT( isPointerAligned( initData.vertices.getBegin(), TIKI_ALIGNOF( Vector2 ) ) );
			shape.polygonShape.create( (const Vector2*)initData.vertices.getBegin(), initData.vertices.getCount() );
			pShape = &shape.polygonShape;
			break;

		default:
			TIKI_TRACE_ERROR( "[PhysicsCharacterControllerComponent] Could not initilize State, because of unsupported ShapeType.\n" );
			break;
		}

		return pShape;
	}

	TIKI_INLINE void disposePhysics2dComponentShape( Physics2dComponentShape& shape )
	{
		switch ( shape.shapeType )
		{
		case Physics2dShapeType_Box:
			shape.boxShape.dispose();
			break;

		case Physics2dShapeType_Circle:
			shape.circleShape.dispose();
			break;

		case Physics2dShapeType_Polygon:
			shape.polygonShape.dispose();
			break;

		default:
			TIKI_TRACE_ERROR( "[PhysicsCharacterControllerComponent] Could not dispose State, because of unsupported ShapeType.\n" );
			break;
		}
	}
}
