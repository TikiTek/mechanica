#pragma once
#ifndef __TIKI_STRAM_HPP_INCLUDED__
#define __TIKI_STRAM_HPP_INCLUDED__

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

		virtual void		dispose() TIKI_PURE;

		virtual FileSize	read( void* pTargetData, FileSize bytesToRead ) const TIKI_PURE;
		virtual FileSize	write( const void* pSourceData, FileSize bytesToWrite ) TIKI_PURE;
		
		virtual FileSize	getPosition() const TIKI_PURE;
		virtual void		setPosition( FileSize position ) TIKI_PURE;
		virtual FileSize	seekPosition( FileOffset offset, DataStreamSeek method = DataStreamSeek_Current ) TIKI_PURE;

		virtual FileSize	getLength() const TIKI_PURE;
		virtual void		setLength( FileSize length ) TIKI_PURE;

	};
}

#endif // __TIKI_STRAM_HPP_INCLUDED__
