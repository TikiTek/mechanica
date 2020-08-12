#pragma once
#ifndef __TIKI_PHYSICSCOLLISIONOBJECT_HPP_INCLUDED__
#define __TIKI_PHYSICSCOLLISIONOBJECT_HPP_INCLUDED__

#include "tiki/base/types.hpp"

namespace tiki
{
	class PhysicsCollisionObject
	{
		TIKI_NONCOPYABLE_CLASS( PhysicsCollisionObject );

	public:

						PhysicsCollisionObject()	{}
		virtual			~PhysicsCollisionObject()	{}

		virtual void*	getNativeObject() const = 0;

	};
}

#endif // __TIKI_PHYSICSCOLLISIONOBJECT_HPP_INCLUDED__
