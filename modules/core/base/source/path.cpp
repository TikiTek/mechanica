
#include "tiki/base/path.hpp"

namespace tiki
{
	namespace path
	{
		string checkSlashes( const string& path )
		{
			return path.replace('\\', '/').toLower();
		}

		string getFilename( const string& fullPath )
		{
			string fileName = checkSlashes( fullPath );
			sint32 index = fileName.lastIndexOf( '/' ) + 1;

			return fileName.substring( index, fileName.length() - index );
		}

		string getFilenameWithoutExtension( const string& fullPath )
		{
			string fileName = checkSlashes( fullPath );

			sint32 index = fileName.lastIndexOf( '/' ) + 1;
			fileName = fullPath.substring(index, fileName.length() - index);

			index = fileName.lastIndexOf( '.' );
			fileName = fileName.substring( 0, index );

			return fileName;
		}
		
		string getExtension( const string& fullPath )
		{
			sint32 index = fullPath.lastIndexOf( '.' );
			return fullPath.substring( (uint32)index  );
		}

		string getDirectoryName( const string& fullPath )
		{
			string dirName = checkSlashes( fullPath );
			return dirName.substring( 0, dirName.lastIndexOf( '/' ) );
		}

		string combine( const string& path1, const string& path2 )
		{
			char i1 = path1[ path1.length() - 1u ];
			char i2 = path2[ 0u ];

			int rightV = (i2 == '/' || i2 == '\\' ? 1 : 0);

			string left		= path1.substring( 0, path1.length() - ( i1 == '/' || i1 == '\\' ? 1 : 0 ) );
			string right	= path2.substring( rightV, path2.length() - rightV );

			return checkSlashes( left + "/" + right );
		}
				
		string combine( const string& path1, const string& path2, const string& path3 )
		{
			return combine( path1, combine( path2, path3 ) );
		}
	}
}