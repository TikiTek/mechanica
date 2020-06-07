#pragma once

#include "tiki/base/types.hpp"

class b2Shape;

namespace tiki
{
	class Physics2dShape
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( Physics2dShape );

	public:

		virtual b2Shape*	getNativeShape() const = 0;
	};
}
