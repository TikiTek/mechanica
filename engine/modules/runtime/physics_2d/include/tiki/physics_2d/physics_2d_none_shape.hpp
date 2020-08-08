#pragma once

#include "tiki/physics_2d/physics_2d_shape.hpp"

namespace tiki
{
	class Physics2dNoneShape : public Physics2dShape
	{
	public:

		virtual b2Shape* getNativeShape() const TIKI_OVERRIDE_FINAL
		{
			return nullptr;
		}
	};
}
