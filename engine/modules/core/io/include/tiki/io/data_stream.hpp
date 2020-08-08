#pragma once

#include "tiki/base/types.hpp"

namespace tiki
{
	typedef uint64	FileSize;
	typedef sint64	FileOffset;

	enum DataStreamSeek
	{
		DataStreamSeek_Begin,
		DataStreamSeek_Current,
		DataStreamSeek_End,

		DataStreamSeek_Count
	};

	class DataStream
	{
	public:

		virtual void		dispose() = 0;

		virtual FileSize	read( void* pTargetData, FileSize bytesToRead ) const = 0;
		virtual FileSize	write( const void* pSourceData, FileSize bytesToWrite ) = 0;

		virtual FileSize	getPosition() const = 0;
		virtual void		setPosition( FileSize position ) = 0;
		virtual FileSize	seekPosition( FileOffset offset, DataStreamSeek method = DataStreamSeek_Current ) = 0;

		virtual FileSize	getLength() const = 0;
		virtual void		setLength( FileSize length ) = 0;
	};
}
