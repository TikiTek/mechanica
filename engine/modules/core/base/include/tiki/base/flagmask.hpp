#pragma once
#ifndef TIKI_FLAGMASK_HPP_INCLUDED
#define TIKI_FLAGMASK_HPP_INCLUDED

namespace tiki
{
	template< class TEnum, class TStorage >
	class FlagMask
	{
	public:

		explicit	FlagMask( TEnum value );

		bool		isAnyFlagSet() const;
		bool		isFlagSet( TEnum value ) const;

		void		clear();

		void		setFlag( TEnum value );
		void		clearFlag( TEnum value );

		FlagMask	operator&( const FlagMask& rhs ) const;
		FlagMask	operator|( const FlagMask& rhs ) const;
		FlagMask&	operator&=( TEnum rhs );
		FlagMask&	operator&=( const FlagMask& rhs );
		FlagMask&	operator|=( TEnum rhs );
		FlagMask&	operator|=( const FlagMask& rhs );

	private:

		TStorage	m_value;

		TStorage	getEnumValue( TEnum value ) const;
	};

	template< class TEnum >
	class FlagMask8 : public FlagMask< TEnum, uint8 >
	{
	public:

		explicit FlagMask8( TEnum value )
			: FlagMask( value )
		{
		}
	};

	template< class TEnum >
	class FlagMask16 : public FlagMask< TEnum, uint16 >
	{
	public:

		explicit FlagMask16( TEnum value )
			: FlagMask( value )
		{
		}
	};

	template< class TEnum >
	class FlagMask32 : public FlagMask< TEnum, uint32 >
	{
	public:

		explicit FlagMask32( TEnum value )
			: FlagMask( value )
		{
		}
	};

	template< class TEnum >
	class FlagMask64 : public FlagMask< TEnum, uint64 >
	{
	public:

		explicit FlagMask64( TEnum value )
			: FlagMask( value )
		{
		}
	};
}

#include "../../../source/flagmask.inl"

#endif // TIKI_FLAGS_HPP_INCLUDED
