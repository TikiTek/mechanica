
#include "tiki/base/assert.hpp"

namespace tiki
{
	template< typename TState, typename TInitData >
	bool Component<TState, TInitData>::initializeState( ComponentState* pComponentState, const void* pComponentInitData )
	{
		TIKI_ASSERT( pComponentState != nullptr );

		if ( m_pFirstComponentState == nullptr )
		{
			m_pFirstComponentState	= pComponentState;
			m_pLastComponentState	= pComponentState;
		}
		else
		{
			m_pLastComponentState->pNextComponentOfSameType = pComponentState;
			m_pLastComponentState = pComponentState;
		}
				
		return internalInitializeState( (TState*)pComponentState, (TInitData*)pComponentInitData );
	}
		
	template< typename TState, typename TInitData >
	void Component<TState, TInitData>::disposeState( ComponentState* pComponentState )
	{
		TIKI_ASSERT( pComponentState == nullptr );

		if ( pComponentState == m_pFirstComponentState )
		{
			m_pFirstComponentState = m_pFirstComponentState->pNextComponentOfSameType;
		}
		else
		{
			pComponentState->pPrevComponentOfSameType->pNextComponentOfSameType = pComponentState->pNextComponentOfSameType;

			if ( pComponentState == m_pLastComponentState )
			{
				TIKI_ASSERT( pComponentState->pPrevComponentOfSameType->pNextComponentOfSameType == nullptr );
				m_pLastComponentState = pComponentState->pPrevComponentOfSameType;
			}
		}

		internalDisposeState( (TState*)pComponentState );
	}

	template< typename TState, typename TInitData >
	ComponentIterator< TState > Component<TState, TInitData>::getIterator() const
	{
		return Iterator( static_cast< State* >( m_pFirstComponentState ) );
	}

	template< typename TState, typename TInitData >
	ComponentIterator< const TState > Component<TState, TInitData>::getConstIterator() const
	{
		return ConstIterator( static_cast< const State* >( m_pFirstComponentState ) );
	}
}