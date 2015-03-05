#pragma once
#ifndef TIKI_PATH_HPP
#define TIKI_PATH_HPP

#include "tiki/base/basicstring.hpp"

namespace tiki
{
	namespace path
	{
		string	checkSlashes( const string& path );

		string	getFilename( const string& fullPath );
		string	getFilenameWithoutExtension( const string& fullPath );		
		string	getExtension( const string& fullPath );
		string	getDirectoryName( const string& fullPath );

		string	combine( const string& path1, const string& path2 );
		string	combine( const string& path1, const string& path2, const string& path3 );

		string	getAbsolutePath( const string& path );
		string	getCurrentDir();
		string	getExecutablePath();
	}
}

#endif // TIKI_PATH_HPP
