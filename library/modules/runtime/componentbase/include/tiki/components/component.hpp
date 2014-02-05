#pragma once
#ifndef __TIKI_COMPONENTBASE_HPP_INCLUDED__
#define __TIKI_COMPONENTBASE_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/components/componentiterator.hpp"

namespace tiki
{
	class ComponentBase
	{
		TIKI_NONCOPYABLE_CLASS( ComponentBase );

	public:

								ComponentBase();
		virtual					~ComponentBase();

		virtual bool			initializeState( ComponentState* pComponentState, const void* pComponentInitData );
		virtual void			disposeState( ComponentState* pComponentState );

		virtual uint			getStateSize() const = 0;
		virtual ComponentType	getTypeId() const = 0;

	protected:

		ComponentState*			m_pFirstComponentState;
		ComponentState*			m_pLastComponentState;

	};

	template< typename TState, typename TInitData >
	class Component : public ComponentBase
	{
		TIKI_NONCOPYABLE_CLASS( Component );

	public:
		
		typedef TState								State;
		typedef TInitData							InitData;

		typedef ComponentIterator< TState >			Iterator;
		typedef ComponentIterator< const TState >	ConstIterator;

						Component() {}
		virtual			~Component() {}

		Iterator		getIterator() const;
		ConstIterator	getConstIterator() const;

	protected:

		virtual bool	internalInitializeState( TState* pComponentState, const TInitData* pComponentInitData ) = 0;
		virtual void	internalDisposeState( TState* pComponentState ) = 0;

	};
}

#include "../../../source/component.inl"

#endif // __TIKI_COMPONENTBASE_HPP_INCLUDED__
