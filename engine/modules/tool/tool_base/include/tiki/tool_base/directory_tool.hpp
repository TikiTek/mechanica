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
		bool	getFiles( List< DynamicString >& targetList, const DynamicString& path );
		bool	getDirectories( List< DynamicString >& targetList, const DynamicString& path );

		bool	findFiles( List< Path >& targetList, const Path& path, const DynamicString& extension, bool recursive = true );
		bool	findFiles( List< Path >& targetList, const DynamicString& path, const DynamicString& extension, bool recursive = true );
	}
}

#endif // TIKI_DIRECTORY_TOOL_HPP
