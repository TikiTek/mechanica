#pragma once
#ifndef TIKI_DIRECTORY_TOOL_HPP
#define TIKI_DIRECTORY_TOOL_HPP

#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/list.hpp"

namespace tiki
{
	class Path;

	namespace directory
	{
		bool	getFiles( List< string >& targetList, const string& path );
		bool	getDirectories( List< string >& targetList, const string& path );

		void	findFiles( List< Path >& targetList, const Path& path, const string& extension, bool recursive = true );
		void	findFiles( List< Path >& targetList, const string& path, const string& extension, bool recursive = true );
	}
}

#endif // TIKI_DIRECTORY_TOOL_HPP
