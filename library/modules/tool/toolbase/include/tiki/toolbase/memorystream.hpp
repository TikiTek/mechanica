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
		MemoryStream( const MemoryStream& copy );
		~MemoryStream();

		void		operator=( const MemoryStream& copy );

		void		create( uint capacity = 0u );
		void		create( const void* pData, uint length );
		void		dispose();

		bool		isOpen() const;
		const void*	getData() const;

		uint		read( void* pData, uint length ) const;
		
		void		write( const void* pData, uint length );
		void		writeAlignment( uint alignment );

		uint		getLength() const;

		uint		getPosition() const;
		uint		setPosition( uint pos );

	private:

		uint8*		m_pData;

		uint		m_pos;
		uint		m_length;
		uint		m_capacity;

	};
}

#endif // TIKI_MEMORYSTREAM_HPP
