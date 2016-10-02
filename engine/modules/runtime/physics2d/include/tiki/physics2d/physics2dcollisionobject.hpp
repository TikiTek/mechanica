#pragma once
#ifndef TIKI_PHYSICS2DCOLLISIONOBJECT_HPP_INCLUDED
#define TIKI_PHYSICS2DCOLLISIONOBJECT_HPP_INCLUDED

#include "tiki/base/types.hpp"

namespace tiki
{
	class Physics2dCollisionObject
	{
		TIKI_NONCOPYABLE_CLASS( Physics2dCollisionObject );

	public:

						Physics2dCollisionObject()	{}
		virtual			~Physics2dCollisionObject()	{}

		virtual void*	getNativeObject() const TIKI_PURE;
	};
}

#endif // TIKI_PHYSICS2DCOLLISIONOBJECT_HPP_INCLUDED
