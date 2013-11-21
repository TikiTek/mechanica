#pragma once
#ifndef __TIKI_SORTEDSIZEDMAP_HPP_INCLUDED__
#define __TIKI_SORTEDSIZEDMAP_HPP_INCLUDED__

#include "tiki/base/types.hpp"

namespace tiki
{
	template<typename TKey, typename TValue>
	class SortedSizedMap
	{
		TIKI_NONCOPYABLE_CLASS( SortedSizedMap );

	public:

		struct Pair
		{
			TKey	key;
			TValue	value;
		};

		SortedSizedMap();
		~SortedSizedMap();

		void		create( uint size, uint alignment = TIKI_DEFAULT_ALIGNMENT );
		void		dispose();

		uint		getCount();
		uint		getCapacity();

		void		getValueAt( TValue* pTargetValue, uint index );
		bool		findValue( TValue* pTargetValue, TKey key );

		void		set( const TKey& key, const TValue& value );

		bool		remove( const TKey& key );

	private:

		Pair*		m_pData;
		uint		m_count;
		uint		m_capacity;

		uint		findIndex( const TKey& key ) const;

	};
}

#include "../../../source/sortedsizedmap.inl"

#endif // __TIKI_SORTEDSIZEDMAP_HPP_INCLUDED__
