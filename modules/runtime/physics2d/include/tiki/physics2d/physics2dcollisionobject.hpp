#pragma once
#ifndef TIKI_PHYSICS2DCOLLISIONOBJECT_HPP_INCLUDED
#define TIKI_PHYSICS2DCOLLISIONOBJECT_HPP_INCLUDED

#include "tiki/base/types.hpp"

class b2Body;

namespace tiki
{
	class Physics2dCollisionObject
	{
		TIKI_NONCOPYABLE_CLASS( Physics2dCollisionObject );

	public:

						Physics2dCollisionObject()	{}
		virtual			~Physics2dCollisionObject()	{}

		virtual b2Body*	getNativeObject() const TIKI_PURE;
	};
}

#endif // TIKI_PHYSICS2DCOLLISIONOBJECT_HPP_INCLUDED
