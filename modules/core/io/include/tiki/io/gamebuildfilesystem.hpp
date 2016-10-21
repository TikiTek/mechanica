#pragma once
#ifndef __TIKI_GAMEBUILDFILESYSTEM_HPP_INCLUDED__
#define __TIKI_GAMEBUILDFILESYSTEM_HPP_INCLUDED__

#include "tiki/io/filesystem.hpp"

#include "tiki/container/array.hpp"
#include "tiki/container/linkedlist.hpp"
#include "tiki/io/filestream.hpp"

namespace tiki
{
	class GamebuildFileSystem : public FileSystem
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( GamebuildFileSystem );

	public:

		bool				create( const char* pGamebuildPath, uint maxStreamCount = 4u );
		void				dispose();

		virtual const char*	getFilenameByCrc( crc32 filenameCrc ) const TIKI_OVERRIDE_FINAL;

		virtual bool		exists( const char* pFileName ) const TIKI_OVERRIDE_FINAL;
		virtual DataStream*	open( const char* pFileName, DataAccessMode accessMode ) TIKI_OVERRIDE_FINAL;

	private:

		struct GamebuildFile : LinkedItem< GamebuildFile >
		{
			crc32	filenameCrc;
			char	aFileName[ 1u ];
		};
		typedef LinkedList< GamebuildFile > GamebuildFileList;

		char				m_gamebuildPath[ TIKI_MAX_PATH ];
		GamebuildFileList	m_files;

		Array< FileStream >	m_fileStreams;

	};
}

#endif // __TIKI_GAMEBUILDFILESYSTEM_HPP_INCLUDED__
