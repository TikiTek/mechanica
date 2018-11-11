#pragma once

#include "tiki/container/key_value_pair.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	template<typename TKey, typename TValue>
	class Map
	{
	public:

		typedef KeyValuePair< TKey, TValue >		Pair;
		typedef KeyValuePair< TKey, TValue >*		Iterator;
		typedef const KeyValuePair< TKey, TValue >*	ConstIterator;

		TIKI_FORCE_INLINE					Map();
		TIKI_FORCE_INLINE					Map( const Map< TKey, TValue >& copy );
		TIKI_FORCE_INLINE					~Map();

		TIKI_FORCE_INLINE void				dispose();

		TIKI_FORCE_INLINE void				clear();

		TIKI_FORCE_INLINE uint				getCount() const;
		TIKI_FORCE_INLINE bool				isEmpty() const;

		TIKI_FORCE_INLINE bool				hasKey( const TKey& key ) const;

		TIKI_FORCE_INLINE TValue&			getValueAt( uint index );
		TIKI_FORCE_INLINE const TValue&		getValueAt( uint index ) const;
		TIKI_FORCE_INLINE Pair&				getPairAt( uint index );
		TIKI_FORCE_INLINE const Pair&		getPairAt( uint index ) const;

		TIKI_FORCE_INLINE bool				findValue( TValue* pTargetValue, const TKey& key ) const;

		TIKI_FORCE_INLINE TValue&			set( const TKey& key, const TValue& value );
		TIKI_FORCE_INLINE bool				remove( const TKey& key );

		TIKI_FORCE_INLINE Iterator			getBegin();
		TIKI_FORCE_INLINE ConstIterator		getBegin() const;

		TIKI_FORCE_INLINE Iterator			getEnd();
		TIKI_FORCE_INLINE ConstIterator		getEnd() const;

		TIKI_FORCE_INLINE TValue&			operator[]( const TKey& key );
		TIKI_FORCE_INLINE const TValue&		operator[]( const TKey& key ) const;

		TIKI_FORCE_INLINE void				operator=( const Map< TKey, TValue >& copy );

		TIKI_FORCE_INLINE Iterator			begin() { return getBegin(); }
		TIKI_FORCE_INLINE ConstIterator		begin() const { return getBegin(); }

		TIKI_FORCE_INLINE Iterator			end() { return getEnd(); }
		TIKI_FORCE_INLINE ConstIterator		end() const { return getEnd(); }

	private:

		Pair*					m_pData;
		uint					m_count;
		uint					m_capacity;

		TIKI_FORCE_INLINE uint	findIndex( const TKey& key ) const;

		TIKI_FORCE_INLINE uint	getNextSize( uint targetSize );
		TIKI_FORCE_INLINE void	checkArraySize( uint neddedSize );

	};
}

#include "../../../source/map.inl"
