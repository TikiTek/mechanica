#pragma once
#ifndef TIKI_FLAGMASK_INL_INCLUDED
#define TIKI_FLAGMASK_INL_INCLUDED

#include "tiki/base/assert.hpp"

namespace tiki
{
	template< class TEnum, class TStorage >
	FlagMask<TEnum, TStorage>::FlagMask()
	{
		m_value = 0;
	}

	template< class TEnum, class TStorage >
	FlagMask<TEnum, TStorage>::FlagMask( TEnum value )
	{
		setFlag( value );
	}

	template< class TEnum, class TStorage >
	bool FlagMask<TEnum, TStorage>::isAnyFlagSet() const
	{
		return m_value != 0;
	}

	template< class TEnum, class TStorage >
	bool FlagMask<TEnum, TStorage>::isFlagSet( TEnum value ) const
	{
		const TStorage valueFlag = getEnumValue( value );
		return (m_value & valueFlag) == valueFlag;
	}

	template< class TEnum, class TStorage >
	void FlagMask<TEnum, TStorage>::clear()
	{
		m_value = 0;
	}

	template< class TEnum, class TStorage >
	void FlagMask<TEnum, TStorage>::setFlag( TEnum value )
	{
		m_value |= getEnumValue( value );
	}

	template< class TEnum, class TStorage >
	void FlagMask<TEnum, TStorage>::clearFlag( TEnum value )
	{
		m_value &= ~getEnumValue( value );
	}

	template< class TEnum, class TStorage >
	FlagMask<TEnum, TStorage> FlagMask<TEnum, TStorage>::operator&( const FlagMask& rhs ) const
	{
		FlagMask mask;
		mask.m_value = m_value & rhs.m_value;
		return mask;
	}

	template< class TEnum, class TStorage >
	FlagMask<TEnum, TStorage> FlagMask<TEnum, TStorage>::operator|( const FlagMask& rhs ) const
	{
		FlagMask mask;
		mask.m_value = m_value | rhs.m_value;
		return mask;
	}

	template< class TEnum, class TStorage >
	FlagMask<TEnum, TStorage>& FlagMask<TEnum, TStorage>::operator&=( TEnum rhs )
	{
		m_value &= getEnumValue( rhs );
	}

	template< class TEnum, class TStorage >
	FlagMask<TEnum, TStorage>& FlagMask<TEnum, TStorage>::operator&=( const FlagMask& rhs )
	{
		m_value &= rhs.m_value;
	}

	template< class TEnum, class TStorage >
	FlagMask<TEnum, TStorage>& FlagMask<TEnum, TStorage>::operator|=( TEnum rhs )
	{
		m_value |= getEnumValue( rhs );
	}

	template< class TEnum, class TStorage >
	FlagMask<TEnum, TStorage>& FlagMask<TEnum, TStorage>::operator|=( const FlagMask& rhs )
	{
		m_value &|= rhs.m_value;
	}

	template< class TEnum, class TStorage >
	TStorage FlagMask<TEnum, TStorage>::getEnumValue( TEnum value ) const
	{
		TIKI_ASSERT( 64u - countLeadingZeros64( (uint64)value ) >= sizeof( TStorage ) * 8u );
		return TStorage( 1 ) << TStorage( value );
	}
}

#endif // TIKI_FLAGMASK_INL_INCLUDED
