#pragma once

#include "tiki/base/types.hpp"

namespace tiki
{
#if TIKI_ENABLED( TIKI_PLATFORM_WIN )
	using uint32_atomic = volatile uint32;
#elif TIKI_ENABLED( TIKI_BUILD_GCC ) || TIKI_ENABLED( TIKI_BUILD_CLANG )
	using uint32_atomic = uint32;
#else
#	error Platform not supported
#endif

	template< class T >
	struct AtomicTypeTraits
	{
		typedef T ValueType;

		static ValueType	toValueType( T value ) { return value; }
		static T			fromValueType( ValueType value ) { return value; }
	};

	template<>
	struct AtomicTypeTraits< bool >
	{
		typedef uint32_atomic ValueType;

		static ValueType	toValueType( bool value ) { return value ? 1u : 0u; }
		static bool			fromValueType( ValueType value ) { return value != 0u ? true : false; }
	};

	template<>
	struct AtomicTypeTraits< uint32 >
	{
		typedef uint32_atomic ValueType;

		static ValueType	toValueType( uint32 value ) { return value; }
		static uint32		fromValueType( ValueType value ) { return value; }
	};

	template< class T >
	struct AtomicAccesorTraits
	{
	};

	template<>
	struct AtomicAccesorTraits< uint32_atomic >
	{
		static uint32	loadRelaxed( const uint32_atomic* pValue );
		static uint32	loadAquire( const uint32_atomic* pValue );
		static uint32	loadOrdered( const uint32_atomic* pValue );

		static void		storeRelaxed( uint32_atomic* pValue, uint32 newValue );
		static void		storeRelease( uint32_atomic* pValue, uint32 newValue );
		static void		storeOrdered( uint32_atomic* pValue, uint32 newValue );

		static uint32	orRelaxed( uint32_atomic* pValue, uint32 mask );
		static uint32	orRelease( uint32_atomic* pValue, uint32 mask );
		static uint32	orOrdered( uint32_atomic* pValue, uint32 mask );

		static uint32	andRelaxed( uint32_atomic* pValue, uint32 mask );
		static uint32	andRelease( uint32_atomic* pValue, uint32 mask );
		static uint32	andOrdered( uint32_atomic* pValue, uint32 mask );
	};

	template< class T >
	class Atomic
	{
	public:

		typedef AtomicTypeTraits< T >				ValueTraits;
		typedef typename ValueTraits::ValueType		ValueType;
		typedef AtomicAccesorTraits< ValueType >	AccessorTraits;

		Atomic();

		T			getRelaxed() const;
		T			getAquire() const;
		T			getOrdered() const;

		void		setRelaxed( T value );
		void		setRelease( T value );
		void		setOrdered( T value );

		// return previous value
		T			orRelaxed( T value );
		T			orRelease( T value );
		T			orOrdered( T value );

		T			andRelaxed( T value );
		T			andRelease( T value );
		T			andOrdered( T value );

	private:

		ValueType	m_value;
	};
}

#include "../../../source/atomic.inl"
