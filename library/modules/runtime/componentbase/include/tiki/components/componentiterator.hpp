#pragma once
#ifndef __TIKI_COMPONENTITERATOR_HPP_INCLUDED__
#define __TIKI_COMPONENTITERATOR_HPP_INCLUDED__

#include "tiki/components/componentstate.hpp"

namespace tiki
{
	template<typename TState>
	class ComponentIterator
	{
		TIKI_NONCOPYABLE_CLASS( ComponentIterator );

	public:

					ComponentIterator( TState* pFirstState );
					~ComponentIterator();

		bool		next();
		void		reset();

		TState*		getCurrent() const;

	private:

		TState*		m_pFirst;
		TState*		m_pCurrent;

	};
	
	template<typename TState>
	class ConstComponentIterator
	{
		TIKI_NONCOPYABLE_CLASS( ConstComponentIterator );

	public:

						ConstComponentIterator( const TState* pFirstState );
						~ConstComponentIterator();

		bool			next();
		void			reset();

		const TState*	getCurrent() const;

	private:

		const TState*	m_pFirst;
		const TState*	m_pCurrent;

	};
}

#include "../../../source/componentiterator.inl"

#endif // __TIKI_COMPONENTITERATOR_HPP_INCLUDED__
