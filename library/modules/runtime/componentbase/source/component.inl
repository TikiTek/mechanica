
#include "tiki/base/assert.hpp"

namespace tiki
{
	template< typename TState, typename TInitData >
	bool Component<TState, TInitData>::initializeState( ComponentEntityIterator& componentIterator, ComponentState* pComponentState, const void* pComponentInitData )
	{
		TIKI_ASSERT( pComponentState != nullptr );

		if ( m_pFirstComponentState == nullptr )
		{
			m_pFirstComponentState	= pComponentState;
		}
				
		return internalInitializeState( componentIterator, (TState*)pComponentState, (TInitData*)pComponentInitData );
	}
		
	template< typename TState, typename TInitData >
	void Component<TState, TInitData>::disposeState( ComponentState* pComponentState )
	{
		TIKI_ASSERT( pComponentState != nullptr );

		if ( pComponentState == m_pFirstComponentState )
		{
			m_pFirstComponentState = m_pFirstComponentState->pNextComponentOfSameType;
		}

		internalDisposeState( (TState*)pComponentState );
	}

	template< typename TState, typename TInitData >
	ComponentTypeIterator< TState > Component<TState, TInitData>::getIterator() const
	{
		return Iterator( static_cast< State* >( m_pFirstComponentState ) );
	}

	template< typename TState, typename TInitData >
	ComponentTypeIterator< const TState > Component<TState, TInitData>::getConstIterator() const
	{
		return ConstIterator( static_cast< const State* >( m_pFirstComponentState ) );
	}
}