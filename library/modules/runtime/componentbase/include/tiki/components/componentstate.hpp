#pragma once
#ifndef __TIKI_COMPONENTSTATE_HPP_INCLUDED__
#define __TIKI_COMPONENTSTATE_HPP_INCLUDED__

#include "tiki/components/component_types.hpp"

namespace tiki
{
	struct ComponentState
	{
		EntityId		entityId;
		ComponentTypeId	typeId;

		ComponentState*	pPrevComponentOfSameType;
		ComponentState*	pNextComponentOfSameType;
		ComponentState*	pNextComponentOfSameEntity;
	};
}

#endif // __TIKI_COMPONENTSTATE_HPP_INCLUDED__
