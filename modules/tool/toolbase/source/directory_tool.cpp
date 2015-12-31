
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
			iterator.create( path.cStr() );
			
			while ( iterator.findNextFile() )
			{
				if( file::exists( iterator.getCurrentFileName() ) )
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
			iterator.create( path.cStr() );
			
			while ( iterator.findNextFile() )
			{
				if( directory::exists( iterator.getCurrentFileName() ) )
				{
					targetList.add( iterator.getCurrentFileName() );
				}
			}
			
			iterator.dispose();

			return true;
		}
	}
}