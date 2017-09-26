#include "tiki/toolbase/directory_tool.hpp"

#include "tiki/base/path.hpp"
#include "tiki/io/directory.hpp"
#include "tiki/io/directoryiterator.hpp"
#include "tiki/io/path.hpp"

namespace tiki
{
	bool directory::getFiles( List< string >& targetList, const string& path )
	{
		DirectoryIterator iterator;
		if( !iterator.create( path.cStr() ) )
		{
			return false;
		}

		while( iterator.findNextFile() )
		{
			if( iterator.isCurrentFile() )
			{
				targetList.add( iterator.getCurrentFileName() );
			}
		}

		iterator.dispose();

		return true;
	}

	bool directory::getDirectories( List< string >& targetList, const string& path )
	{
		DirectoryIterator iterator;
		if( !iterator.create( path.cStr() ) )
		{
			return false;
		}

		while( iterator.findNextFile() )
		{
			if( iterator.isCurrentDirectory() )
			{
				targetList.add( iterator.getCurrentFileName() );
			}
		}

		iterator.dispose();

		return true;
	}

	void directory::findFiles( List< Path >& targetList, const Path& path, const string& extension, bool recursive /*= true */ )
	{
		findFiles( targetList, path.getCompletePath(), extension, recursive );
	}

	void directory::findFiles( List< Path >& targetList, const string& path, const string& extension, bool recursive /* = true */ )
	{
		List< string > dirFiles;
		directory::getFiles( dirFiles, path );
		for( size_t i = 0u; i < dirFiles.getCount(); ++i )
		{
			if( path::getExtension( dirFiles[ i ] ) != extension )
			{
				continue;
			}

			Path& file = targetList.add();
			file.setCombinedPath( path.cStr(), dirFiles[ i ].cStr() );
		}

		if( !recursive )
		{
			return;
		}

		List< string > dirDirectories;
		directory::getDirectories( dirDirectories, path );
		for( size_t i = 0u; i < dirDirectories.getCount(); ++i )
		{
			const string subDirectory = path::combine( path, dirDirectories[ i ] );
			findFiles( targetList, subDirectory, extension, true );
		}
	}
}