#pragma once
#ifndef TIKI_DIRECTORY_HPP
#define TIKI_DIRECTORY_HPP

#include "tiki/base/array.hpp"
#include "tiki/base/basicstring.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	namespace directory
	{
		bool	create( const string& path );
		bool	remove( const string& path );

		bool	exists( const string& path );
	}
}

#endif // TIKI_DIRECTORY_HPP