#pragma once
#ifndef __TIKI_QUEUE_HPP_INCLUDED__
#define __TIKI_QUEUE_HPP_INCLUDED__

#include "tiki/base/types.hpp"

namespace tiki
{
	template< typename T >
	class Queue
	{
		TIKI_NONCOPYABLE_CLASS( Queue );

	public:

		using			Type			= T;
		using			Iterator		= T*;
		using			ConstIterator	= const T*;
		using			Reference		= T&;
		using			ConstReference	= const T&;

						Queue();
						~Queue();

		bool			create( uintreg capacity );
		void			dispose();

		bool			isEmpty() const;
		bool			isFull() const;

		Reference		push();
		Reference		push( ConstReference value );

		bool			pop( Reference target );

		Reference		getTop();
		ConstReference	getTop() const;
		Reference		getBottom();
		ConstReference	getBottom() const;

		uintreg			getCount() const;
		uintreg			getCapacity() const;

	private:

		T*				m_pData;
		uintreg			m_top;
		uintreg			m_bottom;
		uintreg			m_capacity;
	};
}

#include "../../../source/queue.inl"

#endif // __TIKI_QUEUE_HPP_INCLUDED__
