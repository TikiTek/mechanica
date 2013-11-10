#pragma once
#ifndef TIKI_MEMORYSTREAM_HPP
#define TIKI_MEMORYSTREAM_HPP

#include "tiki/base/types.hpp"

namespace tiki
{
	class MemoryStream
	{
	public:

		MemoryStream();
		~MemoryStream();

		void		create( size_t capacity = 0u );
		void		create( const void* pData, size_t length );
		void		dispose();

		bool		isOpen() const;
		const void*	getData() const;

		size_t		read( void* pData, size_t length ) const;
		
		void		write( const void* pData, size_t length );
		void		writeAlignment( uint alignment );

		size_t		getLength() const;

		size_t		getPosition() const;
		size_t		setPosition( size_t pos );

	private:

		uint8*		m_pData;

		size_t		m_pos;
		size_t		m_length;
		size_t		m_capacity;

	};
}

#endif // TIKI_MEMORYSTREAM_HPP
