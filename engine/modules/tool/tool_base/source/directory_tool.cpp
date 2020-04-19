#include "tiki/tool_base/directory_tool.hpp"

#include "tiki/base/path.hpp"
#include "tiki/io/directory.hpp"
#include "tiki/io/directory_iterator.hpp"
#include "tiki/io/path.hpp"

namespace tiki
{
	bool directory::getFiles( List< DynamicString >& targetList, const DynamicString& path )
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

	bool directory::getDirectories( List< DynamicString >& targetList, const DynamicString& path )
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

	bool directory::findFiles( List< Path >& targetList, const Path& path, const DynamicString& extension, bool recursive /*= true */ )
	{
		return findFiles( targetList, path.getCompletePath(), extension, recursive );
	}

	bool directory::findFiles( List< Path >& targetList, const DynamicString& path, const DynamicString& extension, bool recursive /* = true */ )
	{
		List< DynamicString > dirFiles;
		if( !directory::getFiles( dirFiles, path ) )
		{
			TIKI_TRACE_ERROR( "[genericdata] Unable to find files in '%s'", path.cStr() );
			return false;
		}

		for( uintreg i = 0u; i < dirFiles.getCount(); ++i )
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
			return true;
		}

		List< DynamicString > dirDirectories;
		if( !directory::getDirectories( dirDirectories, path ) )
		{
			TIKI_TRACE_ERROR( "[genericdata] Unable to find directories in '%s'", path.cStr() );
			return false;
		}

		for( uintreg i = 0u; i < dirDirectories.getCount(); ++i )
		{
			const DynamicString subDirectory = path::combine( path, dirDirectories[ i ] );
			if( !findFiles( targetList, subDirectory, extension, true ) )
			{
				return false;
			}
		}

		return true;
	}
}