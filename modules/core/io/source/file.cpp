#include "tiki/io/file.hpp"

namespace tiki
{
	bool file::writeToFileIfNotEquals( const Path& filePath, const string& content )
	{
		AutoArray< char > currentContent;
		file::readAllText( filePath.getCompletePath(), currentContent );
		const uint currentContentLength = currentContent.getCount() - 1;

		bool isEquals = (content.getLength() == currentContentLength);
		if( isEquals )
		{
			for( uint i = 0u; i < content.getLength() && isEquals; ++i )
			{
				isEquals &= content[ i ] == currentContent[ i ];
			}
		}

		if( !isEquals )
		{
			return file::writeAllBytes( filePath.getCompletePath(), (const uint8*)content.cStr(), content.getLength() );
		}

		return true;
	}
}