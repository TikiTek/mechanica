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
		reset();
	}

	TIKI_FORCE_INLINE ComponentEntityIterator::~ComponentEntityIterator()
	{
		m_pFirst	= nullptr;
		m_pCurrent	= nullptr;
	}

	TIKI_FORCE_INLINE ComponentState* ComponentEntityIterator::getNext()
	{
		ComponentState* pState = m_pCurrent;

		if ( m_pCurrent != nullptr )
		{
			m_pCurrent = m_pCurrent->pNextComponentOfSameEntity;
		}

		return pState;
	}

	TIKI_FORCE_INLINE void ComponentEntityIterator::reset()
	{
		m_pCurrent = m_pFirst;
	}
}

#endif // __TIKI_COMPONENTITERATOR_INL_INCLUDED__
