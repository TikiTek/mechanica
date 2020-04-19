#pragma once

#include "tiki/base/assert.hpp"
#include "tiki/base/memory.hpp"

namespace tiki
{
	template< typename TState, typename TInitData >
	Component<TState, TInitData>::Component( crc32 typeCrc, const char* pTypeName, uint32 stateSize, bool constructState )
		: ComponentBase( typeCrc, pTypeName, stateSize, constructState )
	{
	}

	template< typename TState, typename TInitData >
	bool Component<TState, TInitData>::initializeState( const ComponentEntityIterator& iterator, ComponentState* pState, const ComponentInitData* pInitData )
	{
		TIKI_ASSERT( pState != nullptr );

		if( pInitData == nullptr )
		{
			return false;
		}

		TState* pTypedState = (TState*)pState;
		if( m_constuctState )
		{
			constructComponentState( pTypedState );
		}

		if( !internalInitializeState( iterator, pTypedState, (const TInitData*)pInitData ) )
		{
			if( m_constuctState )
			{
				destructComponentState( pTypedState );
			}
			return false;
		}

		if ( m_pFirstState == nullptr )
		{
			m_pFirstState = pState;
		}

		return true;
	}

	template< typename TState, typename TInitData >
	void Component<TState, TInitData>::disposeState( ComponentState* pState )
	{
		TIKI_ASSERT( pState != nullptr );

		if ( pState == m_pFirstState )
		{
			m_pFirstState = m_pFirstState->pNextComponentOfSameType;
		}

		TState* pTypedState = (TState*)pState;
		internalDisposeState( pTypedState );

		if( m_constuctState )
		{
			destructComponentState( pTypedState );
		}
	}

#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
	TIKI_FORCE_INLINE bool checkStateIntegrity( const ComponentState* pState )
	{
		if ( pState == nullptr )
		{
			return true;
		}

		if ( pState->entityId == InvalidEntityId )
		{
			TIKI_TRACE_ERROR( "[component] State(0x%p) has a invalid entityId.\n", pState );
			return false;
		}

		if ( pState->typeId == InvalidComponentTypeId )
		{
			TIKI_TRACE_ERROR( "[component] State(0x%p) has a invalid typeId.\n", pState );
			return false;
		}

		return true;
	}

	template< typename TState, typename TInitData >
	bool Component<TState, TInitData>::checkIntegrity() const
	{
		ComponentTypeIterator< const ComponentState > iterator = ComponentTypeIterator< const ComponentState >( (const ComponentState*)m_pFirstState );

		const ComponentState* pState = nullptr;
		while ( pState = iterator.getNext() )
		{
			if ( !checkStateIntegrity( pState ) )
			{
				return false;
			}

			if ( !checkStateIntegrity( pState->pPrevComponentOfSameType ) )
			{
				return false;
			}

			if ( !checkStateIntegrity( pState->pNextComponentOfSameType ) )
			{
				return false;
			}

			if( !checkStateIntegrity( pState->pPrevComponentOfSameEntity ) )
			{
				return false;
			}

			if ( !checkStateIntegrity( pState->pNextComponentOfSameEntity ) )
			{
				return false;
			}
		}

		return true;
	}
#endif

	template< typename TState, typename TInitData >
	ComponentTypeIterator< TState > Component<TState, TInitData>::getIterator() const
	{
		return Iterator( static_cast< State* >( m_pFirstState ) );
	}

	template< typename TState, typename TInitData >
	ComponentTypeIterator< const TState > Component<TState, TInitData>::getConstIterator() const
	{
		return ConstIterator( static_cast< const State* >( m_pFirstState ) );
	}

	template< typename TState, typename TInitData >
	TState* tiki::Component<TState, TInitData>::getFirstStateOfEntity( const ComponentEntityIterator& entityComponentIterator )
	{
		return (TState*)entityComponentIterator.getFirstOfType( m_registedTypeId );
	}

	template< typename TState, typename TInitData >
	const TState* tiki::Component<TState, TInitData>::getFirstStateOfEntity( const ComponentEntityIterator& entityComponentIterator ) const
	{
		return (const TState*)entityComponentIterator.getFirstOfType( m_registedTypeId );
	}
}
