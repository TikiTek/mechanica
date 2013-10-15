#pragma once
#ifndef TIKI_MEMORY_HPP
#define TIKI_MEMORY_HPP

#include "tiki/base/inline.hpp"
#include "tiki/base/types.hpp"
#include "tiki/base/assert.hpp"

#define TIKI_DECLARE_STACKANDZERO( type, name ) type name; \
	memory::zero( &name, sizeof( type ) )

namespace tiki
{
	namespace memory
	{
		void*					allocAlign( size_t size, size_t alignment = TIKI_DEFAULT_ALIGNMENT );
		void					freeAlign( void* pPtr );

		int						cmp ( void* ptr1, void* ptr2, size_t bytes );
		void					set( void* p, size_t size, uint8 value );
		void					copy( void* pTarget, const void* pSource, size_t length );
		
		template<typename T>
		TIKI_FORCE_INLINE T*	newAlign( size_t alignment = TIKI_DEFAULT_ALIGNMENT );

		template<typename T>
		TIKI_FORCE_INLINE void	deleteAlign( T* p );

		template<typename T>
		TIKI_FORCE_INLINE T*	newArray( size_t count, size_t alignment = TIKI_DEFAULT_ALIGNMENT );

		template<typename T>
		TIKI_FORCE_INLINE void	deleteArray( T* pArray, size_t arraySize );

		template<typename T>
		TIKI_FORCE_INLINE void	set( T* p, uint8 value );

		template<typename T>
		TIKI_FORCE_INLINE void	zero( T& p );

		TIKI_FORCE_INLINE void	zero( void* p, size_t size );

	}
}

#include "../../../source/memory.inl"

#endif // TIKI_MEMORY_HPP
