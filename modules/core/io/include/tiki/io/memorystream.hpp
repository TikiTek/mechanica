#pragma once
#ifndef TIKI_MEMORYSTREAM_HPP
#define TIKI_MEMORYSTREAM_HPP

#include "tiki/io/datastream.hpp"

#include "tiki/base/types.hpp"

namespace tiki
{
	class MemoryStream : public DataStream
	{
	public:

					MemoryStream();
					MemoryStream( const MemoryStream& copy );
		virtual		~MemoryStream();

		void		operator=( const MemoryStream& copy );

		void				create( FileSize capacity = 0u );
		void				create( const void* pData, FileSize length );
		virtual void		dispose() TIKI_OVERRIDE_FINAL;

		bool				isOpen() const;
		const void*			getData() const;

		virtual FileSize	read( void* pTargetData, FileSize bytesToRead ) const TIKI_OVERRIDE_FINAL;

		virtual FileSize	write( const void* pSourceData, FileSize bytesToWrite ) TIKI_OVERRIDE_FINAL;
		void				writeAlignment( FileSize alignment );

		virtual FileSize	getPosition() const TIKI_OVERRIDE_FINAL;
		virtual void		setPosition( FileSize position ) TIKI_OVERRIDE_FINAL;
		virtual FileSize	seekPosition( FileOffset offset, DataStreamSeek method = DataStreamSeek_Current ) TIKI_OVERRIDE_FINAL;

		virtual FileSize	getLength() const TIKI_OVERRIDE_FINAL;
		virtual void		setLength( FileSize length ) TIKI_OVERRIDE_FINAL;

	private:

		uint8*				m_pData;

		FileSize			m_pos;
		FileSize			m_length;
		FileSize			m_capacity;

		bool				increaseBuffer( FileSize requiredSize );

	};
}

#endif // TIKI_MEMORYSTREAM_HPP
