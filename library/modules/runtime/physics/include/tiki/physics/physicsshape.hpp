#pragma once
#ifndef __TIKI_PHYSICSSHAPE_HPP_INCLUDED__
#define __TIKI_PHYSICSSHAPE_HPP_INCLUDED__

#include "tiki/physics/physicsbody.hpp"

namespace tiki
{
	class PhysicsShape : public PhysicsBody
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( PhysicsShape );

	public:

		virtual void*	getNativeShape() = 0;

	};
}

#endif // __TIKI_PHYSICSSHAPE_HPP_INCLUDED__