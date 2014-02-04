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

								ComponentBase() {}
		virtual					~ComponentBase() {}

		virtual uint			getStateSize() const = 0;
		virtual ComponentType	getTypeId() const = 0;

	};

	template<typename TState, typename TInitData>
	class Component : public ComponentBase
	{
		TIKI_NONCOPYABLE_CLASS( Component );

	public:

		typedef ComponentIterator< TState >			Iterator;
		typedef ConstComponentIterator< TState >	ConstIterator;

						Component() {}
		virtual			~Component() {}

		bool			initializeState( TState* pComponentState, const TInitData* pComponentInitData );
		void			disposeState( TState* pComponentState );

		Iterator		getIterator() const;
		ConstIterator	getConstIterator() const;

	protected:

		virtual bool	internalInitializeState( TState* pComponentState, const TInitData* pComponentInitData ) = 0;
		virtual void	internalDisposeState( TState* pComponentState ) = 0;

	private:

		TState*			m_pFirstComponentState;
		
	};
}

#endif // __TIKI_COMPONENTBASE_HPP_INCLUDED__
