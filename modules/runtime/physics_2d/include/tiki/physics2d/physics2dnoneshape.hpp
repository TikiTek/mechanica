#pragma once
#ifndef TIKI_PHYSICS2DNONESHAPE_HPP_INCLUDED
#define TIKI_PHYSICS2DNONESHAPE_HPP_INCLUDED

#include "tiki/physics2d/physics2dshape.hpp"

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

#endif // TIKI_PHYSICS2DNONESHAPE_HPP_INCLUDED
