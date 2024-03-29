#pragma once

#include "tiki/io/data_stream.hpp"

#include "tiki/base/types.hpp"

namespace tiki
{
	class MemoryStream : public DataStream
	{
		TIKI_NON_COPYABLE( MemoryStream );

	public:

							MemoryStream();
		virtual				~MemoryStream();

		void				create( FileSize capacity = 0u );
		void				create( const void* pData, FileSize length );
		virtual void		dispose() TIKI_OVERRIDE_FINAL;

		bool				isOpen() const;
		const uint8*		getData() const;

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
