namespace tiki
{
	template< int bits >
	tiki::Bitfield<bits>::Bitfield()
	{
		TIKI_ASSERT( bytes > 0 );
		for( int i = 0; i < bytes; m_fields[ i++ ] = 0 );
	}

	template< int bits >
	tiki::Bitfield<bits>::Bitfield( const Bitfield<bits>& field )
	{
		for( int i = 0; i < bytes; m_fields[ i ] = field.m_fields[ i++ ] );
	}

	template< int bytes >
	tiki::Bitfield<bytes>::~Bitfield()
	{

	}

	template< int bits >
	Bitfield<bits>& tiki::Bitfield<bits>::operator=( const Bitfield<bits>& field )
	{
		memory::copy( m_fields, field.m_fields, bytes );
		return *this;
	}

	template< int bits >
	bool tiki::Bitfield<bits>::operator==( Bitfield<bits>& field )
	{
		return memory::cmp( m_fields, field.m_fields, bytes ) == 0;
	}

	template< int bits >
	bool tiki::Bitfield<bits>::operator!=( Bitfield<bits>& field )
	{
		return memory::cmp( m_fields, field.m_fields, bytes ) != 0;
	}

	template< int bits >
	Bitfield<bits>& tiki::Bitfield<bits>::operator |= (  const uint index )
	{
		TIKI_ASSERT( (index >> 3 ) < bytes );
		m_fields[ index >> 3 ] |= ( 1 << ( index & 7 ) );
		return *this;
	}

	template< int bits >
	Bitfield<bits>& tiki::Bitfield<bits>::operator |= ( const Bitfield<bits>& other )
	{
		for( int i = 0; i < bytes; m_fields[ i ] |= other.m_fields[ i++ ] );
		return *this;
	}

	template< int bits >
	bool tiki::Bitfield<bits>::has( const uint bit )
	{
		TIKI_ASSERT( (bit >> 3) < bytes );
		const uint flag = ( 1 << ( bit & 7 ) );
		return ( m_fields[ bit >> 3 ] & flag ) == flag;
	}

	template< int bits >
	bool tiki::Bitfield<bits>::has( const Bitfield<bits>& mask )
	{
		for( int i = 0; i < bytes; ++i )
		{
			if( (m_fields[ i ] & mask.m_fields[ i ]) != mask.m_fields[i] )
				return false;
		}
		return true;
	}

	template< int bits >
	void tiki::Bitfield<bits>::set( const uint bit )
	{
		TIKI_ASSERT( (bit >> 3 ) < bytes );
		m_fields[ bit >> 3 ] |= ( 1 << ( bit & 7 ) );
	}

	template< int bits >
	void tiki::Bitfield<bits>::unset( const uint bit )
	{
		TIKI_ASSERT( (bit >> 3 ) < bytes );
		m_fields[ bit >> 3 ] &= ~( 1 << ( bit & 7 ) );
	}

	template< int bits >
	void tiki::Bitfield<bits>::clear( void )
	{
		for( int i = 0; i < bytes; m_fields[ i++ ] = 0 );
	}
}