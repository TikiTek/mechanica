#pragma once
#ifndef __TIKI_GAMEBUILDFILESYSTEM_HPP_INCLUDED__
#define __TIKI_GAMEBUILDFILESYSTEM_HPP_INCLUDED__

#include "tiki/io/filesystem.hpp"

#include "tiki/base/array.hpp"
#include "tiki/io/filestream.hpp"

namespace tiki
{
	class GamebuildFileSystem : public FileSystem
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( GamebuildFileSystem );

	public:

		virtual bool exists( cstring pFileName ) const;
		virtual DataStream* open( cstring pFileName, DataAccessMode accessMode );

	private:

		Array< FileStream >	m_fileStreams;

	};
}

#endif // __TIKI_GAMEBUILDFILESYSTEM_HPP_INCLUDED__
