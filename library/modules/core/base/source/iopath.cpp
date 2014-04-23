
#include "tiki/base/iopath.hpp"

#include <windows.h>

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
		const sint32 index = fullPath.lastIndexOf( '.' );
		if ( index == -1 )
		{
			return "";
		}

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
	
	string path::getAbsolutePath( const string& path )
	{
		const string slashPath = checkSlashes( path );

		const bool beginWithDrive	= ( slashPath.substring( 1u, 2u ) == ":/" );
		const bool containsDots		= ( slashPath.contains( "/../" ) || slashPath.contains( "/./" ) );
		if ( beginWithDrive && !containsDots )
		{
			// is already absolute
			return slashPath;
		}
		string fullPath = slashPath;
		if ( !beginWithDrive )
		{
			fullPath = path::combine( getCurrentDir(), slashPath );
		}
			
		for (;;)
		{
			const int index = fullPath.indexOf( "/.." );
			if ( index <= 0 )
			{
				break;
			}

			const uint lastIndex = (uint)index + 3u;
			const uint prevIndex = (uint)fullPath.lastIndexOf( '/', (uint)index - 1u );
			TIKI_ASSERT( prevIndex < fullPath.getLength() );
			
			fullPath = fullPath.remove( prevIndex, lastIndex - prevIndex );
		}

		return fullPath.replace( "/./", "/" );
	}

	string path::getCurrentDir()
	{
		char buffer[ 256u ];
		GetCurrentDirectoryA( TIKI_COUNT( buffer ), buffer );

		return buffer;
	}

	string path::getExecutablePath()
	{
		HMODULE currentModule = GetModuleHandleA( nullptr );

		char buffer[ 256u ];
		GetModuleFileNameA( currentModule, buffer, TIKI_COUNT( buffer ) );

		return path::getDirectoryName( buffer );
	}
}