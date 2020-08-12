#pragma once
#ifndef __TIKI_COMPONENTITERATOR_HPP_INCLUDED__
#define __TIKI_COMPONENTITERATOR_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/components/componentstate.hpp"

namespace tiki
{
	template<typename TState>
	class ComponentTypeIterator
	{
	public:

		TIKI_FORCE_INLINE			ComponentTypeIterator( TState* pFirstState );
		TIKI_FORCE_INLINE			~ComponentTypeIterator();

		TIKI_FORCE_INLINE TState*	getNext();

		TIKI_FORCE_INLINE void		reset();

	private:

		TState*		m_pFirst;
		TState*		m_pCurrent;

	};
		
	class ComponentEntityIterator
	{
	public:

		TIKI_FORCE_INLINE					ComponentEntityIterator( ComponentState* pFirstState );
		TIKI_FORCE_INLINE					~ComponentEntityIterator();

		TIKI_FORCE_INLINE ComponentState*	getNext();
		TIKI_FORCE_INLINE ComponentState*	getFirstOfType( ComponentTypeId typeId );

		TIKI_FORCE_INLINE void				reset();

	private:

		ComponentState*		m_pFirst;
		ComponentState*		m_pCurrent;

	};
}

#include "../../../source/componentiterator.inl"

#endif // __TIKI_COMPONENTITERATOR_HPP_INCLUDED__
