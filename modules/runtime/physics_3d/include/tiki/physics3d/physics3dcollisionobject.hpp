#pragma once
#ifndef TIKI_PHYSICS3DCOLLISIONOBJECT_HPP_INCLUDED
#define TIKI_PHYSICS3DCOLLISIONOBJECT_HPP_INCLUDED

#include "tiki/base/types.hpp"

namespace tiki
{
	class Physics3dCollisionObject
	{
		TIKI_NONCOPYABLE_CLASS( Physics3dCollisionObject );

	public:

						Physics3dCollisionObject()	{}
		virtual			~Physics3dCollisionObject()	{}

		virtual void*	getNativeObject() const = 0;
	};
}

#endif // TIKI_PHYSICS3DCOLLISIONOBJECT_HPP_INCLUDED
