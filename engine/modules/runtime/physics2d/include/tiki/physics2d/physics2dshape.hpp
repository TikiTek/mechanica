#pragma once
#ifndef TIKI_PHYSICS2DSHAPE_HPP_INCLUDED
#define TIKI_PHYSICS2DSHAPE_HPP_INCLUDED

#include "tiki/base/types.hpp"

class b2Shape;

namespace tiki
{
	class Physics2dShape
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( Physics2dShape );

	public:

		virtual b2Shape*	getNativeShape() const TIKI_PURE;
	};
}

#endif // TIKI_PHYSICS2DSHAPE_HPP_INCLUDED
