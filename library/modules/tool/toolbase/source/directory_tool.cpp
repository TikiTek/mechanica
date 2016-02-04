
#include "tiki/toolbase/directory_tool.hpp"

#include "tiki/io/directory.hpp"
#include "tiki/io/directoryiterator.hpp"
#include "tiki/io/file.hpp"

namespace tiki
{
	namespace directory
	{
		bool getFiles( const string& path, List< string >& targetList )
		{
			DirectoryIterator iterator;
			if ( !iterator.create( path.cStr() ) )
			{
				return false;
			}
			
			while ( iterator.findNextFile() )
			{
				if( iterator.isCurrentFile() )
				{
					targetList.add( iterator.getCurrentFileName() );
				}
			}
			
			iterator.dispose();

			return true;
		}

		bool getDirectories( const string& path, List< string >& targetList )
		{
			DirectoryIterator iterator;
			if ( !iterator.create( path.cStr() ) )
			{
				return false;
			}
			
			while ( iterator.findNextFile() )
			{
				if( iterator.isCurrentDirectory() )
				{
					targetList.add( iterator.getCurrentFileName() );
				}
			}
			
			iterator.dispose();

			return true;
		}
	}
}