#pragma once
#ifndef __TIKI_COMPONENTITERATOR_INL_INCLUDED__
#define __TIKI_COMPONENTITERATOR_INL_INCLUDED__

namespace tiki
{
	// ComponentIterator
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
	bool ComponentIterator<TState>::next()
	{
		m_pCurrent = m_pCurrent->pNextComponentOfSameType;
		return ( m_pCurrent != nullptr );
	}

	template<typename TState>
	void ComponentIterator<TState>::reset()
	{
		m_pCurrent = m_pFirst;
	}

	template<typename TState>
	TState* ComponentIterator<TState>::getCurrent() const
	{
		return m_pCurrent;
	}

	// ConstComponentIterator
	template<typename TState>
	ConstComponentIterator<TState>::ConstComponentIterator( const TState* pFirstState )
	{
		m_pFirst = pFirstState;
		reset();
	}

	template<typename TState>
	ConstComponentIterator<TState>::~ConstComponentIterator()
	{
		m_pFirst	= nullptr;
		m_pCurrent	= nullptr;
	}

	template<typename TState>
	bool ConstComponentIterator<TState>::next()
	{
		m_pCurrent = m_pCurrent->pNextComponentOfSameType;
		return ( m_pCurrent != nullptr );
	}

	template<typename TState>
	void ConstComponentIterator<TState>::reset()
	{
		m_pCurrent = m_pFirst;
	}

	template<typename TState>
	const TState* ConstComponentIterator<TState>::getCurrent() const
	{
		return m_pCurrent;
	}
}

#endif // __TIKI_COMPONENTITERATOR_INL_INCLUDED__
