#pragma once
#ifndef TIKI_PHYSICS3DSHAPE_HPP_INCLUDED
#define TIKI_PHYSICS3DSHAPE_HPP_INCLUDED

#include "tiki/physics3d/physics3dbody.hpp"

namespace tiki
{
	class Physics3dShape
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( Physics3dShape );

	public:

		virtual void*	getNativeShape() const = 0;
	};
}

#endif // TIKI_PHYSICS3DSHAPE_HPP_INCLUDED
