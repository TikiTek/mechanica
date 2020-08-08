#include "tiki/io/file.hpp"

namespace tiki
{
	FileWriteResult file::writeToFileIfNotEquals( const Path& filePath, const DynamicString& content )
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
			const bool result = file::writeAllBytes( filePath.getCompletePath(), (const uint8*)content.cStr(), content.getLength() );
			return result ? FileWriteResult_Ok : FileWriteResult_Error;
		}

		return FileWriteResult_NoChanged;
	}
}