#pragma once
#ifndef __TIKI_COMPONENTITERATOR_INL_INCLUDED__
#define __TIKI_COMPONENTITERATOR_INL_INCLUDED__

namespace tiki
{
	// ComponentTypeIterator
	template<typename TState>
	TIKI_FORCE_INLINE ComponentTypeIterator<TState>::ComponentTypeIterator( TState* pFirstState )
	{
		m_pFirst = pFirstState;
		reset();
	}

	template<typename TState>
	TIKI_FORCE_INLINE ComponentTypeIterator<TState>::~ComponentTypeIterator()
	{
		m_pFirst	= nullptr;
		m_pCurrent	= nullptr;
	}

	template<typename TState>
	TIKI_FORCE_INLINE TState* ComponentTypeIterator<TState>::getNext()
	{
		TState* pState = m_pCurrent;

		if ( m_pCurrent != nullptr )
		{
			m_pCurrent = static_cast< TState* >( m_pCurrent->pNextComponentOfSameType );
		}

		return pState;
	}

	template<typename TState>
	TIKI_FORCE_INLINE void ComponentTypeIterator<TState>::reset()
	{
		m_pCurrent = m_pFirst;
	}

	// ComponentEntityIterator
	TIKI_FORCE_INLINE ComponentEntityIterator::ComponentEntityIterator( ComponentState* pFirstState )
	{
		m_pFirst = pFirstState;
	}

	TIKI_FORCE_INLINE ComponentEntityIterator::~ComponentEntityIterator()
	{
		m_pFirst	= nullptr;
	}

	TIKI_FORCE_INLINE ComponentState* ComponentEntityIterator::getFirstOfType( ComponentTypeId typeId ) const
	{
		ComponentState* pState = m_pFirst;
		while ( pState != nullptr )
		{
			if ( pState->typeId == typeId )
			{
				return pState;
			}

			pState = pState->pNextComponentOfSameEntity;
		}

		return nullptr;
	}
}

#endif // __TIKI_COMPONENTITERATOR_INL_INCLUDED__
