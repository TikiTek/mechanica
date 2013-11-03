
#include "tiki/base/iopath.hpp"

namespace tiki
{
	string path::checkSlashes( const string& path )
	{
		return path.replace('\\', '/').toLower();
	}

	string path::getFilename( const string& fullPath )
	{
		string fileName = checkSlashes( fullPath );
		sint32 index = fileName.lastIndexOf( '/' ) + 1;

		return fileName.substring( index, fileName.getLength() - index );
	}

	string path::getFilenameWithoutExtension( const string& fullPath )
	{
		string fileName = checkSlashes( fullPath );

		sint32 index = fileName.lastIndexOf( '/' ) + 1;
		fileName = fullPath.substring(index, fileName.getLength() - index);

		index = fileName.lastIndexOf( '.' );
		fileName = fileName.substring( 0, index );

		return fileName;
	}
		
	string path::getExtension( const string& fullPath )
	{
		sint32 index = fullPath.lastIndexOf( '.' );
		return fullPath.substring( (uint32)index  );
	}

	string path::getDirectoryName( const string& fullPath )
	{
		string dirName = checkSlashes( fullPath );
		return dirName.substring( 0, dirName.lastIndexOf( '/' ) );
	}

	string path::combine( const string& path1, const string& path2 )
	{
		char i1 = path1[ path1.getLength() - 1u ];
		char i2 = path2[ 0u ];

		int rightV = (i2 == '/' || i2 == '\\' ? 1 : 0);

		string left		= path1.substring( 0, path1.getLength() - ( i1 == '/' || i1 == '\\' ? 1 : 0 ) );
		string right	= path2.substring( rightV, path2.getLength() - rightV );

		return checkSlashes( left + "/" + right );
	}
				
	string path::combine( const string& path1, const string& path2, const string& path3 )
	{
		return combine( path1, combine( path2, path3 ) );
	}

#if TIKI_ENABLED( TIKI_BUILD_TOOLS )
	string path::getProjectDir()
	{
		return nullptr;
	}
#endif
}