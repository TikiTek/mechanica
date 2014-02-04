#pragma once
#ifndef __TIKI_COMPONENTSTATE_HPP_INCLUDED__
#define __TIKI_COMPONENTSTATE_HPP_INCLUDED__

#include "tiki/components/component_types.hpp"

namespace tiki
{
	struct ComponentStateBase
	{
		EntityId		entityId;
		ComponentType	componentType;
	};
	
	template<typename T>
	struct ComponentState : public ComponentStateBase
	{
		T*		pNextComponentOfSameType;
		T*		pNextComponentOfSameEntity;
	};
}

#endif // __TIKI_COMPONENTSTATE_HPP_INCLUDED__
