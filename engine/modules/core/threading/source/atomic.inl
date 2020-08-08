#pragma once

namespace tiki
{
	template< class T >
	Atomic< T >::Atomic()
	{
		setOrdered( T() );
	}

	template< class T >
	T Atomic< T >::getRelaxed() const
	{
		return AccessorTraits::loadRelaxed( &m_value );
	}

	template< class T >
	T Atomic< T >::getAquire() const
	{
		return AccessorTraits::loadAquire( &m_value );
	}

	template< class T >
	T Atomic< T >::getOrdered() const
	{
		return AccessorTraits::loadOrdered( &m_value );
	}

	template< class T >
	void Atomic< T >::setRelaxed( T value )
	{
		AccessorTraits::storeRelaxed( &m_value, ValueTraits::toValueType( value ) );
	}

	template< class T >
	void Atomic< T >::setRelease( T value )
	{
		AccessorTraits::storeRelease( &m_value, ValueTraits::toValueType( value ) );
	}

	template< class T >
	void Atomic< T >::setOrdered( T value )
	{
		AccessorTraits::storeOrdered( &m_value, ValueTraits::toValueType( value ) );
	}

	template< class T >
	T Atomic< T >::orRelaxed( T value )
	{
		return ValueTraits::fromValueType( AccessorTraits::orRelaxed( &m_value, ValueTraits::toValueType( value ) ) );
	}

	template< class T >
	T Atomic< T >::orRelease( T value )
	{
		return ValueTraits::fromValueType( AccessorTraits::orRelease( &m_value, ValueTraits::toValueType( value ) ) );
	}

	template< class T >
	T Atomic< T >::orOrdered( T value )
	{
		return ValueTraits::fromValueType( AccessorTraits::orOrdered( &m_value, ValueTraits::toValueType( value ) ) );
	}

	template< class T >
	T Atomic< T >::andRelaxed( T value )
	{
		return ValueTraits::fromValueType( AccessorTraits::andRelaxed( &m_value, ValueTraits::toValueType( value ) ) );
	}

	template< class T >
	T Atomic< T >::andRelease( T value )
	{
		return ValueTraits::fromValueType( AccessorTraits::andRelease( &m_value, ValueTraits::toValueType( value ) ) );
	}

	template< class T >
	T Atomic< T >::andOrdered( T value )
	{
		return ValueTraits::fromValueType( AccessorTraits::andOrdered( &m_value, ValueTraits::toValueType( value ) ) );
	}
}