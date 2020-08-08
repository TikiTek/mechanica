#pragma once
#ifndef __TIKI_AUTODISPOSE_HPP_INCLUDED__
#define __TIKI_AUTODISPOSE_HPP_INCLUDED__

#include "tiki/base/assert.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	template<typename T>
	struct AutoDispose
	{
	public:

		AutoDispose()
		{
		}

		AutoDispose( const T& value )
		{
			TIKI_ASSERT( m_value.isCreated() == false );
			m_value = m_value;
		}

		AutoDispose( const AutoDispose< T >& ad )
		{
			TIKI_ASSERT( m_value.isCreated() == false );
			m_value = ad.m_value;
		}

		~AutoDispose()
		{
			m_value.dispose();
		}

		void operator=( const AutoDispose< T >& ad )
		{
			TIKI_ASSERT( m_value.isCreated() == false );
			m_value = ad.m_value;
		}

		T& operator*()
		{
			return m_value;
		}

		T*	operator->()
		{
			return &m_value;
		}

		const T* operator->() const
		{
			return &m_value;
		}

	private:

		T	m_value;

	};
}

#endif // __TIKI_AUTODISPOSE_HPP_INCLUDED__
