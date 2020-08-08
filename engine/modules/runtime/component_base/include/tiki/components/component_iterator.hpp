#pragma once

#include "tiki/base/types.hpp"
#include "tiki/components/component_state.hpp"

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

		TIKI_FORCE_INLINE ComponentState*	getFirstOfType( ComponentTypeId typeId ) const;

	private:

		ComponentState*		m_pFirst;
	};
}

#include "../../../source/component_iterator.inl"
