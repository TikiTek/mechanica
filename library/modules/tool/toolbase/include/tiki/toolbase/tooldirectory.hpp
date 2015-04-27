#pragma once
#ifndef TIKI_TOOLDIRECTORY_HPP
#define TIKI_TOOLDIRECTORY_HPP

#include "tiki/base/basicstring.hpp"
#include "tiki/base/types.hpp"
#include "tiki/toolbase/list.hpp"

namespace tiki
{
	namespace directory
	{
		bool	getFiles( const string& path, List< string >& targetList );
		bool	getDirectories( const string& path, List< string >& targetList );
	}
}

#endif // TIKI_TOOLDIRECTORY_HPP
