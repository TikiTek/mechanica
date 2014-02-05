#pragma once
#ifndef __TIKI_COMPONENTSTATE_HPP_INCLUDED__
#define __TIKI_COMPONENTSTATE_HPP_INCLUDED__

#include "tiki/components/component_types.hpp"

namespace tiki
{
	struct ComponentState
	{
		EntityId		entityId;
		ComponentType	componentType;

		ComponentState*	pPrevComponentOfSameType;
		ComponentState*	pNextComponentOfSameType;
		ComponentState*	pNextComponentOfSameEntity;
	};
	
	//template<typename TState>
	//struct ComponentState : public ComponentStateBase
	//{
	//	TState*		pPrevComponentOfSameType;
	//	TState*		pNextComponentOfSameType;
	//	void*		pNextComponentOfSameEntity;
	//};
}

#endif // __TIKI_COMPONENTSTATE_HPP_INCLUDED__
