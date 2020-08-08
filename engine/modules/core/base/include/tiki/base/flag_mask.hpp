#pragma once

namespace tiki
{
	template< class TEnum, class TStorage >
	class FlagMask
	{
	public:

					FlagMask();
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
	using FlagMask8 = FlagMask< TEnum, uint8 >;

	template< class TEnum >
	using FlagMask16 = FlagMask< TEnum, uint16 >;

	template< class TEnum >
	using FlagMask32 = FlagMask< TEnum, uint32 >;

	template< class TEnum >
	using FlagMask64 = FlagMask< TEnum, uint64 >;
}

#include "../../../source/flag_mask.inl"
