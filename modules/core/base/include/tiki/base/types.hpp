#pragma once
#ifndef TIKI_TYPES_HPP
#define TIKI_TYPES_HPP

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

	typedef unsigned __int32	crc32;
	typedef unsigned __int32	fourcc;

#	if TIKI_ENABLED( TIKI_BUILD_64BIT )
	typedef __int64				sint;
	typedef unsigned __int64	uint;
	typedef unsigned __int64	size_t;
#	else
	typedef __int32				sint;
	typedef unsigned __int32	uint;
	typedef unsigned __int32	size_t;
#	endif

#elif TIKI_ENABLED( TIKI_PLATFORM_LINUX )

	typedef unsigned char		uint8;
	typedef unsigned short		uint16;
	typedef unsigned int		uint32;
	typedef unsigned long long int	uint64;

	typedef char				sint8;
	typedef short				sint16;
	typedef int					sint32;
	typedef long long int		sint64;

	typedef unsigned int		crc32;
	typedef unsigned int		fourcc;

#	if TIKI_ENABLED( TIKI_BUILD_64BIT )
	typedef long long int		sint;
	typedef unsigned long long int	uint;
	typedef unsigned long long int	size_t;
#	else
	typedef int					sint;
	typedef unsigned int		uint;
	typedef unsigned int		size_t;
#	endif

#	define nullptr 0

#else

#	error Platform not supported (or you must rebuild solution)

#endif

	typedef uint16				float16;
	typedef float				float32;
	typedef double				float64;
	
	typedef const char*			cstring;
	typedef const wchar_t*		wcstring;
}

#define TIKI_COUNT( var )					(sizeof(var) / sizeof(*var))
#define TIKI_OFFSETOF( type, member )		((uint)&reinterpret_cast<const volatile char&>((((type*)0)->member)))
//#define TIKI_CONTAINEROF( type, ptr, atr )	(type*)((size_t)ptr - TIKI_OFFSETOF( type, atr ))

#if TIKI_ENABLED( TIKI_BUILD_32BIT )
#	define TIKI_SIZE_T_MAX	0xffffffff
#	define TIKI_SIZE_T_BITS 32
#elif TIKI_ENABLED( TIKI_BUILD_64BIT )
#	define TIKI_SIZE_T_MAX	0xffffffffffffffff
#	define TIKI_SIZE_T_BITS 64
#else
#	error Platform not suppored
#endif

#define TIKI_NONCOPYABLE_CLASS( class_name )		\
	private:										\
		class_name ( const class_name & );			\
		void operator=( const class_name & )

#define TIKI_NONCOPYABLE_WITHCTOR_CLASS( class_name ) public:	\
		class_name () {}										\
		~ class_name() {}										\
																\
	private:													\
		class_name ( const class_name & );						\
		void operator=( const class_name & )

#define TIKI_NONCOPYABLE_WITHCTOR_STRUCT( class_name ) public:	\
	private:													\
	class_name ( const class_name & );							\
	void operator=( const class_name & );						\
	public:														\
	class_name () {}											\
	~ class_name() {}											\

#define TIKI_DEFINE_HANLE( handle_name )		\
	struct handle_name ## Type;					\
	typedef handle_name ## Type* handle_name

#if TIKI_ENABLED( TIKI_PLATFORM_WIN ) && TIKI_DISABLED( TIKI_BUILD_MASTER )
#	include <crtdbg.h>
#	define TIKI_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#	define TIKI_DEL delete
#	define TIKI_DELARR delete[]
#else
#	define TIKI_NEW new
#	define TIKI_DEL delete
#	define TIKI_DELARR delete[]
#endif

#define TIKI_CONCAT( x1, x2 ) TIKI_CONCAT_HELPER( x1, x2 )
#define TIKI_CONCAT_HELPER( x1, x2 ) x1 ## x2
#define TIKI_STRING( text ) # text

#define TIKI_DEFAULT_ALIGNMENT 0xffffffffu

#if TIKI_ENABLED( TIKI_PLATFORM_WIN )

#	define TIKI_PRE_ALIGN( var )	__declspec( align( var ) )
#	define TIKI_POST_ALIGN( var )
#	define TIKI_ALIGNOF( type )		( __alignof( type ) )

#else

#	error not supported

#endif



#endif // TIKI_TYPES_HPP
