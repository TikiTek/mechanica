#pragma once
#ifndef __TIKI_SORTEDSIZEDMAP_HPP_INCLUDED__
#define __TIKI_SORTEDSIZEDMAP_HPP_INCLUDED__

#include "tiki/base/types.hpp"

namespace tiki
{
	template<typename TKey, typename TValue>
	struct KeyValuePair
	{
		TKey	key;
		TValue	value;
	};

	template<typename TKey, typename TValue>
	class SortedSizedMap
	{
		TIKI_NONCOPYABLE_CLASS( SortedSizedMap );

	public:

		typedef KeyValuePair<TKey, TValue> Pair;


		SortedSizedMap();
		~SortedSizedMap();

		void			create( uint size, uint alignment = TIKI_DEFAULT_ALIGNMENT );
		void			dispose();

		uint			getCount();
		uint			getCapacity();

		TValue&			getValueAt( uint index );
		const TValue&	getValueAt( uint index ) const;
		Pair&			getPairAt( uint index );
		const Pair&		getPairAt( uint index ) const;

		bool		findValue( TValue* pTargetValue, TKey key ) const;

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
