#pragma once
#ifndef __TIKI_STRAM_HPP_INCLUDED__
#define __TIKI_STRAM_HPP_INCLUDED__

namespace tiki
{
	typedef uint64	FileSize;

	class Stream
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( Stream );

	public:

		virtual FileSize	read( void* pTargetData, uint bytesToRead ) = 0;
		virtual FileSize	write( const void* pSourceData, uint bytesToRead ) = 0;
		
		virtual FileSize	getPosition() = 0;
		virtual FileSize	setPosition( FileSize position ) = 0;
		virtual FileSize	seekPosition( FileSize seek ) = 0u;

		virtual FileSize	getLength() = 0;
		virtual void		setLength( FileSize length ) = 0;

	};
}

#endif // __TIKI_STRAM_HPP_INCLUDED__
