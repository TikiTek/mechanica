#pragma once
#ifndef __TIKI_STRAM_HPP_INCLUDED__
#define __TIKI_STRAM_HPP_INCLUDED__

#include "tiki/base/types.hpp"

namespace tiki
{
	typedef uint64	FileSize;

	class DataStream
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( DataStream );

	public:

		virtual void		close() TIKI_PURE;

		virtual FileSize	read( void* pTargetData, uint bytesToRead ) TIKI_PURE;
		virtual FileSize	write( const void* pSourceData, uint bytesToRead ) TIKI_PURE;
		
		virtual FileSize	getPosition() TIKI_PURE;
		virtual void		setPosition( FileSize position ) TIKI_PURE;
		virtual FileSize	seekPosition( FileSize seek ) TIKI_PURE;

		virtual FileSize	getLength() const TIKI_PURE;
		virtual void		setLength( FileSize length ) TIKI_PURE;

	};
}

#endif // __TIKI_STRAM_HPP_INCLUDED__
