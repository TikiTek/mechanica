#pragma once
#ifndef __TIKI_QUEUE_HPP_INCLUDED__
#define __TIKI_QUEUE_HPP_INCLUDED__

#include "tiki/base/types.hpp"

namespace tiki
{
	template<typename T>
	class Queue
	{
		TIKI_NONCOPYABLE_CLASS( Queue );

	public:

		typedef T			Type;

		typedef T*			Iterator;
		typedef const T*	ConstIterator;

		typedef T&			Reference;
		typedef const T&	ConstReference;

		Queue();
		~Queue();

		bool			create( uint capacity, uint alignment = TIKI_DEFAULT_ALIGNMENT );
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
		
		uint			getCount() const;
		uint			getCapacity() const;

	private:

		T*		m_pData;
		uint	m_top;
		uint	m_bottom;
		uint	m_capacity;

	};
}

#include "../../../source/queue.inl"

#endif // __TIKI_QUEUE_HPP_INCLUDED__
