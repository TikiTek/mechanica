
#include "tiki/base/directory.hpp"

#include <Windows.h>

namespace tiki
{
	namespace directory
	{
		bool create( const string& path )
		{
			return CreateDirectoryA( path.cStr(), nullptr ) != 0;
		}

		bool remove( const string& path )
		{
			return RemoveDirectoryA( path.cStr() ) != 0;
		}

		bool exists( const string& path )
		{
			DWORD att = GetFileAttributesA( path.cStr() );
			return ( att != INVALID_FILE_ATTRIBUTES && ( att & FILE_ATTRIBUTE_DIRECTORY ) != 0 );
		}

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