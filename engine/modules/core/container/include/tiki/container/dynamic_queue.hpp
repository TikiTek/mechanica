#pragma once

#include "tiki/base/types.hpp"

namespace tiki
{
	template< class T >
	class DynamicQueue : NonCopyable
	{
	public:

						DynamicQueue();
						~DynamicQueue();

		void			dispose();

		bool			isEmpty() const;
		bool			hasElements() const;
		uintreg			getLength() const;
		uintreg			getCapacity() const;

		void			clear();
		void			reserve( uintreg size );

		T&				pushBack();
		void			pushBack( const T& value );

		void			popFront();
		bool			tryPopFront( T& target );

		T&				getFront();
		const T&		getFront() const;

		T&				operator[]( uintreg index );
		const T&		operator[]( uintreg index ) const;

	private:

		T*				m_pData		= nullptr;
		uintreg			m_top		= 0u;
		uintreg			m_bottom	= 0u;
		uintreg			m_length	= 0u;
		uintreg			m_capacity	= 0u;

		void			checkCapacity( uintreg capacity );
	};
}

#include "../../../source/dynamic_queue.inl"
