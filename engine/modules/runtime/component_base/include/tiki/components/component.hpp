#pragma once

#include "tiki/base/types.hpp"
#include "tiki/components/component_iterator.hpp"

namespace tiki
{
	struct ComponentInitData;

	class ComponentBase
	{
		TIKI_NONCOPYABLE_CLASS( ComponentBase );

	public:

		virtual					~ComponentBase();

		void					registerComponent( ComponentTypeId typeId );
		void					unregisterComponent();

		virtual bool			initializeState( const ComponentEntityIterator& iterator, ComponentState* pState, const ComponentInitData* pInitData ) = 0;
		virtual void			disposeState( ComponentState* pState ) = 0;

		crc32					getTypeCrc() const { return m_typeCrc; }
		const char*				getTypeName() const { return m_pTypeName; }
		uint32					getStateSize() const { return m_stateSize; }

#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
		virtual bool			checkIntegrity() const = 0;
#endif

		ComponentTypeId			getTypeId() const { return m_registedTypeId; }

	protected:

								ComponentBase( crc32 typeCrc, const char* pTypeName, uint32 stateSize, bool constructState );

		ComponentState*			m_pFirstState;

		crc32					m_typeCrc;
		const char*				m_pTypeName;
		uint32					m_stateSize;
		bool					m_constuctState;

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

		virtual			~Component() {}

		virtual bool	initializeState( const ComponentEntityIterator& iterator, ComponentState* pState, const ComponentInitData* pInitData ) TIKI_OVERRIDE_FINAL;
		virtual void	disposeState( ComponentState* pState ) TIKI_OVERRIDE_FINAL;

#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
		virtual bool	checkIntegrity() const TIKI_OVERRIDE_FINAL;
#endif

		Iterator		getIterator() const;
		ConstIterator	getConstIterator() const;

		TState*			getFirstStateOfEntity( const ComponentEntityIterator& entityComponentIterator );
		const TState*	getFirstStateOfEntity( const ComponentEntityIterator& entityComponentIterator ) const;

	protected:

						Component( crc32 typeCrc, const char* pTypeName, uint32 stateSize, bool constructState );

		virtual bool	internalInitializeState( const ComponentEntityIterator& iterator, TState* pState, const TInitData* pInitData ) = 0;
		virtual void	internalDisposeState( TState* pState ) = 0;
	};
}

#include "../../../source/component.inl"
