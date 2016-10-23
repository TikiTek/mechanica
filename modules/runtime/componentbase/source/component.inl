#pragma once
#ifndef TIKI_COMPONENT_INL_INCLUDED
#define TIKI_COMPONENT_INL_INCLUDED

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
	bool Component<TState, TInitData>::initializeState( ComponentEntityIterator& componentIterator, ComponentState* pComponentState, const void* pComponentInitData )
	{
		TIKI_ASSERT( pComponentState != nullptr );

		TState* pTypedComponentState = (TState*)pComponentState;
		if( m_constuctState )
		{
			constructComponentState( pTypedComponentState );
		}

		if (!internalInitializeState( componentIterator, pTypedComponentState, (TInitData*)pComponentInitData ))
		{
			if( m_constuctState )
			{
				destructComponentState( pTypedComponentState );
			}
			return false;
		}

		if ( m_pFirstComponentState == nullptr )
		{
			m_pFirstComponentState	= pComponentState;
		}
				
		return true;
	}
		
	template< typename TState, typename TInitData >
	void Component<TState, TInitData>::disposeState( ComponentState* pComponentState )
	{
		TIKI_ASSERT( pComponentState != nullptr );

		if ( pComponentState == m_pFirstComponentState )
		{
			m_pFirstComponentState = m_pFirstComponentState->pNextComponentOfSameType;
		}

		TState* pTypedComponentState = (TState*)pComponentState;
		internalDisposeState( pTypedComponentState );

		if( m_constuctState )
		{
			destructComponentState( pTypedComponentState );
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
		ComponentTypeIterator< const ComponentState > iterator = ComponentTypeIterator< const ComponentState >( (const ComponentState*)m_pFirstComponentState );

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
		return Iterator( static_cast< State* >( m_pFirstComponentState ) );
	}

	template< typename TState, typename TInitData >
	ComponentTypeIterator< const TState > Component<TState, TInitData>::getConstIterator() const
	{
		return ConstIterator( static_cast< const State* >( m_pFirstComponentState ) );
	}
}

#endif // TIKI_COMPONENT_INL_INCLUDED
