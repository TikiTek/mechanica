#pragma once

#include "tiki/base/defines.hpp"

namespace tiki
{
#if TIKI_ENABLED( TIKI_BUILD_MSVC )

	typedef unsigned __int8		uint8;
	typedef unsigned __int16	uint16;
	typedef unsigned __int32	uint32;
	typedef unsigned __int64	uint64;

	typedef __int8				sint8;
	typedef __int16				sint16;
	typedef __int32				sint32;
	typedef __int64				sint64;

#elif TIKI_ENABLED( TIKI_BUILD_GCC ) || TIKI_ENABLED( TIKI_BUILD_CLANG )

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

#if TIKI_ENABLED( TIKI_BUILD_32BIT )

	typedef sint32			sint;
	typedef uint32			uint;
	typedef uint32			size_t;

#elif TIKI_ENABLED( TIKI_BUILD_64BIT )

	typedef sint64			sint;
	typedef uint64			uint;
	typedef uint64			size_t;

#else
#	error Platform not suppored
#endif

	typedef uint32			crc32;
	typedef uint32			fourcc;
	typedef sint64			timems;	// time in milliseconds

	typedef uint16			float16;
	typedef float			float32;
	typedef double			float64;

#define TIKI_COUNT( var )					( sizeof( var ) / sizeof( *var ) )

#if TIKI_ENABLED( TIKI_BUILD_GCC ) || TIKI_ENABLED( TIKI_BUILD_CLANG )

#	define TIKI_OFFSETOF( type, member )		( __builtin_offsetof( type, member ) )

#else

#	define TIKI_OFFSETOF( type, member )		( (uint)(&((type*)nullptr)->member) )

#endif

#define TIKI_DEFAULT_ALIGNMENT			0u
#define TIKI_INVALID_CRC32				0xffffffffu
#define TIKI_MAX_PATH					512u
#define TIKI_TIME_OUT_INFINITY			0x7fffffffffffffff

#define TIKI_CONCAT( x1, x2 )			TIKI_CONCAT_HELPER( x1, x2 )
#define TIKI_CONCAT_HELPER( x1, x2 )	x1 ## x2
#define TIKI_STRING( text )				# text

#define TIKI_PURE						= 0


#if TIKI_ENABLED( TIKI_BUILD_32BIT )

#	define TIKI_SIZE_T_MAX		0xffffffffu
#	define TIKI_SIZE_T_BITS		32u
#	define TIKI_SIZE_T_BYTES	4u

#elif TIKI_ENABLED( TIKI_BUILD_64BIT )

#	define TIKI_SIZE_T_MAX		0xffffffffffffffffu
#	define TIKI_SIZE_T_BITS		64u
#	define TIKI_SIZE_T_BYTES	8u

#else

#	error Platform not suppored

#endif

#define TIKI_USE_INLINE TIKI_ON

#if TIKI_ENABLED( TIKI_USE_INLINE )

#	if TIKI_ENABLED( TIKI_BUILD_MSVC )

#		define TIKI_INLINE			inline
#		define TIKI_FORCE_INLINE	inline //__forceinline
#		define TIKI_NO_INLINE		__declspec(noinline)

#	elif TIKI_ENABLED( TIKI_BUILD_GCC ) || TIKI_ENABLED( TIKI_BUILD_CLANG )

#		define TIKI_INLINE			inline
#		define TIKI_FORCE_INLINE	inline //__attribute__((always_inline))
#		define TIKI_NO_INLINE		__attribute__((noinline))

#	else

#		error Platform not implemented

#	endif

#else

#	define TIKI_INLINE
#	define TIKI_FORCE_INLINE
#	define TIKI_NO_INLINE

#endif

#if TIKI_ENABLED( TIKI_BUILD_MSVC )

#	define TIKI_ALIGN_PREFIX( var )		__declspec( align( var ) )
#	define TIKI_ALIGN_POSTFIX( var )
#	define TIKI_ALIGNOF( type )			( __alignof( type ) )

#	define TIKI_CPP_11					TIKI_ON
#	define TIKI_CPP_14					TIKI_ON
#	define TIKI_CPP_17					TIKI_OFF

#	define TIKI_OVERRIDE				override
#	define TIKI_FINAL					sealed

#elif TIKI_ENABLED( TIKI_BUILD_GCC ) || TIKI_ENABLED( TIKI_BUILD_CLANG )

#	define TIKI_ALIGN_PREFIX( var )
#	define TIKI_ALIGN_POSTFIX( var )	__attribute__( ( aligned( var ) ) )
#	define TIKI_ALIGNOF( type )			( __alignof__( type ) )

#	if __cplusplus >= 201103L
#		define TIKI_CPP_11					TIKI_ON
#	else
#		define TIKI_CPP_11					TIKI_OFF
#	endif

#	if __cplusplus >= 201402L
#		define TIKI_CPP_14					TIKI_ON
#	else
#		define TIKI_CPP_14					TIKI_OFF
#	endif

#	if __cplusplus >= 201703L
#		define TIKI_CPP_17					TIKI_ON
#	else
#		define TIKI_CPP_17					TIKI_OFF
#	endif

#if TIKI_ENABLED( TIKI_CPP_11 )
#	define TIKI_OVERRIDE				override
#	define TIKI_FINAL					final
#else
#	define TIKI_OVERRIDE
#	define TIKI_FINAL
#endif

#endif

#define TIKI_OVERRIDE_FINAL				TIKI_OVERRIDE TIKI_FINAL

#if TIKI_DISABLED( TIKI_CPP_11 )
#	define TIKI_NONCOPYABLE_CLASS( class_name )		\
		private:										\
			class_name ( const class_name & );			\
			void operator=( const class_name & )

#	define TIKI_NONCOPYABLE_WITHCTOR_CLASS( class_name )	\
		 public:											\
			class_name () { }								\
			~ class_name () { }								\
		private:											\
			class_name ( const class_name & );				\
			void operator=( const class_name & )

#	define TIKI_NONCOPYABLE_STRUCT( class_name )	\
		private:									\
			class_name ( const class_name & );		\
			void operator=( const class_name & );	\
		public:

#	define TIKI_NONCOPYABLE_WITHCTOR_STRUCT( class_name )	\
		private:											\
			class_name ( const class_name & );				\
			void operator=( const class_name & );			\
		public:												\
			class_name () { }								\
			~ class_name () { }
#else
	struct NonCopyable
	{
		NonCopyable() = default;
		NonCopyable( const NonCopyable& ) = delete;
		NonCopyable&	operator=( const NonCopyable& ) = delete;
	};

#	define TIKI_NON_COPYABLE( type_name )				\
		type_name ( const type_name & ) = delete;		\
		void operator=( const type_name & ) = delete

#	define TIKI_NON_COPYABLE_CONSTRUCTABLE( type_name )	\
		TIKI_NON_COPYABLE( type_name );					\
		type_name () = default

#	define TIKI_NONCOPYABLE_CLASS( type_name )				TIKI_NON_COPYABLE( type_name )

#	define TIKI_NONCOPYABLE_WITHCTOR_CLASS( type_name )	\
		public:											\
			type_name () = default;						\
		private:										\
			TIKI_NON_COPYABLE( type_name )

#	define TIKI_NONCOPYABLE_STRUCT( type_name )				TIKI_NON_COPYABLE( type_name )

#	define TIKI_NONCOPYABLE_WITHCTOR_STRUCT( type_name )	TIKI_NON_COPYABLE_CONSTRUCTABLE( type_name )
#endif

#define TIKI_DEFINE_HANLE( handle_name )		\
	struct handle_name ## Type;					\
	typedef handle_name ## Type* handle_name;	\
	static const handle_name Invalid ## handle_name = (handle_name)TIKI_SIZE_T_MAX

#define TIKI_FLAGS_ENUM( type )	TIKI_FORCE_INLINE type operator|( type a, type b ) { return (type)(sint64( a ) | sint64( b )); }

#define TIKI_MIN( a, b ) ( a < b ? a : b )
#define TIKI_MAX( a, b ) ( a > b ? a : b )
}
