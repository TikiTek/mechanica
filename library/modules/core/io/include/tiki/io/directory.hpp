#pragma once
#ifndef TIKI_DIRECTORY_HPP
#define TIKI_DIRECTORY_HPP

#include "tiki/base/types.hpp"

namespace tiki
{
	namespace directory
	{
		bool	create( const char* pPath );
		bool	remove( const char* pPath );

		bool	exists( const char* pPath );
	}
}

#endif // TIKI_DIRECTORY_HPP
