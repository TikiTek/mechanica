
#include "tiki/base/assert.hpp"

namespace tiki
{
	ComponentBase::ComponentBase()
	{
		m_pFirstComponentState	= nullptr;
		m_pLastComponentState	= nullptr;
	}

	ComponentBase::~ComponentBase()
	{
		m_pFirstComponentState	= nullptr;
		m_pLastComponentState	= nullptr;
	}


	bool ComponentBase::initializeState( ComponentState* pComponentState, const void* pComponentInitData )
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

		//internalInitializeState( pComponentState, pComponentInitData );
		return false;
	}
		
	void ComponentBase::disposeState( ComponentState* pComponentState )
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

		//internalDisposeState( pComponentState );
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