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

		void					registerComponent( ComponentTypeId typeId );
		void					unregisterComponent();

		virtual bool			initializeState( ComponentEntityIterator& componentIterator, ComponentState* pComponentState, const void* pComponentInitData ) = 0;
		virtual void			disposeState( ComponentState* pComponentState ) = 0;

		virtual crc32			getTypeCrc() const;
		virtual uint32			getStateSize() const = 0;
		virtual const char*		getTypeName() const = 0;

#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
		virtual bool			checkIntegrity() const = 0;
#endif

		ComponentTypeId			getTypeId() const { return m_registedTypeId; }
		
	protected:

		ComponentState*			m_pFirstComponentState;

		ComponentTypeId			m_registedTypeId;

	};

	template< typename TState, typename TInitData >
	class Component : public ComponentBase
	{
		TIKI_NONCOPYABLE_CLASS( Component );

	public:
		
		typedef TState									State;
		typedef TInitData								InitData;

		typedef ComponentTypeIterator< TState >			Iterator;
		typedef ComponentTypeIterator< const TState >	ConstIterator;

						Component() {}
		virtual			~Component() {}

		virtual bool	initializeState( ComponentEntityIterator& componentIterator, ComponentState* pComponentState, const void* pComponentInitData );
		virtual void	disposeState( ComponentState* pComponentState );
		
#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
		virtual bool	checkIntegrity() const;
#endif

		Iterator		getIterator() const;
		ConstIterator	getConstIterator() const;

	protected:

		virtual bool	internalInitializeState( ComponentEntityIterator& componentIterator, TState* pComponentState, const TInitData* pComponentInitData ) = 0;
		virtual void	internalDisposeState( TState* pComponentState ) = 0;

	};
}

#include "../../../source/component.inl"

#endif // __TIKI_COMPONENTBASE_HPP_INCLUDED__
