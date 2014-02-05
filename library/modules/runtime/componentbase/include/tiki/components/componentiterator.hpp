#pragma once
#ifndef __TIKI_COMPONENTITERATOR_HPP_INCLUDED__
#define __TIKI_COMPONENTITERATOR_HPP_INCLUDED__

#include "tiki/components/componentstate.hpp"

namespace tiki
{
	template<typename TState>
	class ComponentIterator
	{
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
}

#include "../../../source/componentiterator.inl"

#endif // __TIKI_COMPONENTITERATOR_HPP_INCLUDED__
