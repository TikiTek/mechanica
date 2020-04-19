#pragma once

#include "tiki/base/dynamic_string.hpp"

namespace tiki
{
	namespace path
	{
		DynamicString	checkSlashes( const DynamicString& path );

		DynamicString	getFilename( const DynamicString& fullPath );
		DynamicString	getFilenameWithoutExtension( const DynamicString& fullPath );
		DynamicString	getExtension( const DynamicString& fullPath );
		DynamicString	getDirectoryName( const DynamicString& fullPath );

		DynamicString	combine( const DynamicString& path1, const DynamicString& path2 );
		DynamicString	combine( const DynamicString& path1, const DynamicString& path2, const DynamicString& path3 );

		//DynamicString	getAbsolutePath( const DynamicString& path );
	}
}
