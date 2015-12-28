#pragma once
#ifndef __TIKI_SORTEDSIZEDMAP_HPP_INCLUDED__
#define __TIKI_SORTEDSIZEDMAP_HPP_INCLUDED__

#include "tiki/container/keyvaluepair.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	template<typename TKey, typename TValue>
	class SortedSizedMap
	{
		TIKI_NONCOPYABLE_CLASS( SortedSizedMap );

	public:

		typedef KeyValuePair<TKey, TValue> Pair;
		
		TIKI_FORCE_INLINE				SortedSizedMap();
		TIKI_FORCE_INLINE				~SortedSizedMap();

		TIKI_FORCE_INLINE bool			create( uint size, uint alignment = TIKI_DEFAULT_ALIGNMENT );
		TIKI_FORCE_INLINE void			dispose();

		TIKI_FORCE_INLINE void			clear();

		TIKI_FORCE_INLINE uint			getCount() const;
		TIKI_FORCE_INLINE uint			getCapacity() const;
		TIKI_FORCE_INLINE bool			isEmpty() const;

		TIKI_FORCE_INLINE TValue&		getValueAt( uint index );
		TIKI_FORCE_INLINE const TValue&	getValueAt( uint index ) const;
		TIKI_FORCE_INLINE Pair&			getPairAt( uint index );
		TIKI_FORCE_INLINE const Pair&	getPairAt( uint index ) const;

		TIKI_FORCE_INLINE bool			findValue( TValue* pTargetValue, TKey key ) const;

		TIKI_FORCE_INLINE void			set( const TKey& key, const TValue& value );
		TIKI_FORCE_INLINE bool			remove( const TKey& key );

	private:

		Pair*					m_pData;
		uint					m_count;
		uint					m_capacity;

		TIKI_FORCE_INLINE uint	findIndex( const TKey& key ) const;

	};
}

#include "../../../source/sortedsizedmap.inl"

#endif // __TIKI_SORTEDSIZEDMAP_HPP_INCLUDED__
