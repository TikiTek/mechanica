#pragma once
#ifndef __TIKI_COMPONENTITERATOR_INL_INCLUDED__
#define __TIKI_COMPONENTITERATOR_INL_INCLUDED__

namespace tiki
{
	template<typename TState>
	ComponentIterator<TState>::ComponentIterator( TState* pFirstState )
	{
		m_pFirst = pFirstState;
		reset();
	}

	template<typename TState>
	ComponentIterator<TState>::~ComponentIterator()
	{
		m_pFirst	= nullptr;
		m_pCurrent	= nullptr;
	}

	template<typename TState>
	TState* ComponentIterator<TState>::getNext()
	{
		TState* pState = m_pCurrent;

		if ( m_pCurrent != nullptr )
		{
			m_pCurrent = static_cast< TState* >( m_pCurrent->pNextComponentOfSameType );
		}

		return pState;
	}

	template<typename TState>
	void ComponentIterator<TState>::reset()
	{
		m_pCurrent = m_pFirst;
	}
}

#endif // __TIKI_COMPONENTITERATOR_INL_INCLUDED__
