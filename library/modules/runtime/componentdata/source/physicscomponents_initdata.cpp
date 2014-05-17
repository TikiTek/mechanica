
#include "tiki/components/physicscomponents_initdata.hpp"

namespace tiki
{
	TIKI_REFLECTION_VALUE_TYPE( PhysicsShapeType,	4u, ValueTypeVariant_UnsignedInteger );
	
	TIKI_REFLECTION_CPPDECLARE( PhysicsBodyComponentInitData );
	TIKI_REFLECTION_CPPDECLARE( PhysicsColliderComponentInitData );
}