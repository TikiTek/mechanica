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
		ComponentState*	pPrevComponentOfSameEntity;
		ComponentState*	pNextComponentOfSameEntity;
	};

	template< class T >
	void constructComponentState( T* pState );

	template< class T >
	void destructComponentState( T* pState );

#define TIKI_COMPONENT_STATE_CONSTRUCT_FUNCTIONS( type_name )		\
	template<>														\
	void constructComponentState< type_name >( type_name* pState )	\
	{																\
		memory::callDefaultConstructor( pState );					\
	}																\
	template<>														\
	void destructComponentState< type_name >( type_name* pState )	\
	{																\
		memory::callDestructor( pState );							\
	}

}

#endif // __TIKI_COMPONENTSTATE_HPP_INCLUDED__
