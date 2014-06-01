#pragma once
#ifndef __TIKI_TYPES_HPP_INCLUDED__
#define __TIKI_TYPES_HPP_INCLUDED__

#include "tiki/base/defines.hpp"

#define TIKI_ENABLED( value ) ( ( value 0 ) == 2 )
#define TIKI_DISABLED( value ) ( ( value 0 ) != 2 )

namespace tiki
{
#if TIKI_ENABLED( TIKI_PLATFORM_WIN )

	typedef unsigned __int8		uint8;
	typedef unsigned __int16	uint16;
	typedef unsigned __int32	uint32;
	typedef unsigned __int64	uint64;

	typedef __int8				sint8;
	typedef __int16				sint16;
	typedef __int32				sint32;
	typedef __int64				sint64;

#elif TIKI_ENABLED( TIKI_PLATFORM_LINUX )

	typedef unsigned char			uint8;
	typedef unsigned short			uint16;
	typedef unsigned int			uint32;
	typedef unsigned long long int	uint64;

	typedef char					sint8;
	typedef short					sint16;
	typedef int						sint32;
	typedef long long int			sint64;

#	define nullptr 0

#else

#	error Platform not supported (or you need to rebuild solution)

#endif

#	if TIKI_ENABLED( TIKI_BUILD_32BIT )

	typedef sint32			sint;
	typedef uint32			uint;
	typedef uint32			size_t;

#	elif TIKI_ENABLED( TIKI_BUILD_64BIT )

	typedef sint64			sint;
	typedef uint64			uint;
	typedef uint64			size_t;

#	else

#	error Platform not suppored

#	endif

	typedef uint32			crc32;
	typedef uint32			fourcc;

	typedef uint16			float16;
	typedef float			float32;
	typedef double			float64;
	
	typedef const char*		cstring;
}

#define TIKI_COUNT( var )					( sizeof( var ) / sizeof( *var ) )
#define TIKI_OFFSETOF( type, member )		( (uint)&reinterpret_cast< const volatile char& >( ( ((type*)nullptr)->member ) ) )

#if TIKI_ENABLED( TIKI_BUILD_32BIT )

#	define TIKI_SIZE_T_MAX		0xffffffffu
#	define TIKI_SIZE_T_BITS		32u

#elif TIKI_ENABLED( TIKI_BUILD_64BIT )

#	define TIKI_SIZE_T_MAX		0xffffffffffffffffu
#	define TIKI_SIZE_T_BITS		64u

#else

#	error Platform not suppored

#endif

#define TIKI_NONCOPYABLE_CLASS( class_name )		\
	private:										\
		class_name ## ( const class_name ## & );	\
		void operator=( const class_name ## & )

#define TIKI_NONCOPYABLE_WITHCTOR_CLASS( class_name )	\
	 public:											\
		class_name ## () { }							\
		~ ## class_name ## () { }						\
	private:											\
		class_name ## ( const class_name ## & );		\
		void operator=( const class_name ## & )

#define TIKI_NONCOPYABLE_WITHCTOR_STRUCT( class_name )	\
	public:												\
		class_name ## () {}								\
		~ ## class_name ## () {}						\
	private:											\
		class_name ## ( const class_name ## & );		\
		void operator=( const class_name ## & )

#define TIKI_DEFINE_HANLE( handle_name )		\
	struct handle_name ## Type;					\
	typedef handle_name ## Type* handle_name

#if TIKI_ENABLED( TIKI_PLATFORM_WIN ) && TIKI_DISABLED( TIKI_BUILD_MASTER )
#	include <crtdbg.h>
#	define TIKI_NEW		new (_NORMAL_BLOCK, __FILE__, __LINE__)
#	define TIKI_DEL		delete
#	define TIKI_DELARR	delete[]
#else
#	define TIKI_NEW		new
#	define TIKI_DEL		delete
#	define TIKI_DELARR	delete[]
#endif

#define TIKI_CONCAT( x1, x2 )			TIKI_CONCAT_HELPER( x1, x2 )
#define TIKI_CONCAT_HELPER( x1, x2 )	x1 ## x2
#define TIKI_STRING( text )				# text

#define TIKI_DEFAULT_ALIGNMENT 0xffffffffu

#if TIKI_ENABLED( TIKI_PLATFORM_WIN )

#	define TIKI_PRE_ALIGN( var )	__declspec( align( var ) )
#	define TIKI_POST_ALIGN( var )
#	define TIKI_ALIGNOF( type )		( __alignof( type ) )

#else

#	define TIKI_PRE_ALIGN( var )
#	define TIKI_POST_ALIGN( var )	__attribute__( ( aligned( var ) ) );
#	define TIKI_ALIGNOF( type )		( __alignof__( type ) )

#endif

#endif // __TIKI_TYPES_HPP_INCLUDED__
