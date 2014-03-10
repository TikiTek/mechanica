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

		TState*		getNext();

		void		reset();

	private:

		TState*		m_pFirst;
		TState*		m_pCurrent;

	};
}

#include "../../../source/componentiterator.inl"

#endif // __TIKI_COMPONENTITERATOR_HPP_INCLUDED__
