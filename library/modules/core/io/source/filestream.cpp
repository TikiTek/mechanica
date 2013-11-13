
#include "tiki/io/filestream.hpp"

namespace tiki
{
	FileSize FileStream::read( void* pTargetData, uint bytesToRead )
	{
		return 0;
	}

	FileSize FileStream::write( const void* pSourceData, uint bytesToRead )
	{
		return 0;
	}

	FileSize FileStream::getPosition()
	{
		return 0;
	}

	FileSize FileStream::setPosition( FileSize position )
	{
		return 0;
	}

	FileSize FileStream::seekPosition( FileSize seek )
	{
		return 0;
	}

	FileSize FileStream::getLength()
	{
		return 0;
	}

	void FileStream::setLength( FileSize length )
	{
	}
}