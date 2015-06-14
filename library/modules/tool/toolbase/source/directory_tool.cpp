
#include "tiki/toolbase/directory_tool.hpp"

#include <Windows.h>

namespace tiki
{
	namespace directory
	{
		bool getFiles( const string& path, List< string >& targetList )
		{
			string searchPattern = path.replace( '\\', '/' );
			searchPattern += ( path.endsWith( '/' ) ? "*.*" : "/*.*" );

			WIN32_FIND_DATAA findData;
			HANDLE hFind = FindFirstFileA( searchPattern.cStr(), &findData );
						
			if ( hFind == INVALID_HANDLE_VALUE )
			{
				return false;
			}

			do
			{
				if ( findData.cFileName[ 0u ] == '.' && ( findData.cFileName[ 1u ] == '.' || strlen( findData.cFileName ) == 1u ) )
				{
					continue;
				}
				
				if ( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
				{
					continue;
				}

				targetList.add( findData.cFileName );
			}
			while ( FindNextFileA( hFind, &findData ) );

			FindClose( hFind );
			return true;
		}

		bool getDirectories( const string& path, List< string >& targetList )
		{
			string searchPattern = path.replace( '\\', '/' );
			searchPattern += ( path.endsWith( '/' ) ? "*.*" : "/*.*" );

			WIN32_FIND_DATAA findData;
			HANDLE hFind = FindFirstFileA( searchPattern.cStr(), &findData );

			if ( hFind == INVALID_HANDLE_VALUE )
			{
				return false;
			}

			do
			{
				if ( findData.cFileName[ 0u ] == '.' && ( findData.cFileName[ 1u ] == '.' || strlen( findData.cFileName ) == 1u ) )
				{
					continue;
				}
				
				if ( ( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == 0u )
				{
					continue;
				}

				targetList.add( findData.cFileName );
			}
			while ( FindNextFileA( hFind, &findData ) );

			FindClose( hFind );
			return true;
		}
	}
}