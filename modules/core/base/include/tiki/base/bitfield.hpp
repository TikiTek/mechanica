#ifndef TIKI_BITFIELD_HPP__
#define TIKI_BITFIELD_HPP__

#include "tiki/base/assert.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	template< int bits = 8 >
	class Bitfield
	{
	public:
							Bitfield( void );
							Bitfield( const Bitfield<bits>& field );
							~Bitfield( void );

		Bitfield<bits>&		operator =  ( const Bitfield<bits>& field );
		bool				operator == ( Bitfield<bits>& field );
		bool				operator != ( Bitfield<bits>& field );

		Bitfield<bits>&		operator |= ( const uint index );
		Bitfield<bits>&		operator |= ( const Bitfield<bits>& other );

		bool				has( const uint bit );
		bool				has( const Bitfield<bits>& mask );
		void				set( const uint bit );
		void				unset( const uint bit );

		void				clear( void );

	private:
		static const int bytes = ( ( bits + 8 -1 ) & -8 ) / 8;
		byte m_fields[ bytes ];	
	};
}

#include "bitfield.inl"

#endif // TIKI_BITFIELD_HPP__
