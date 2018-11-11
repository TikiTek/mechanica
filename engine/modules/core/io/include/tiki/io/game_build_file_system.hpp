#pragma once

#include "tiki/io/file_system.hpp"

#include "tiki/base/path.hpp"
#include "tiki/container/array.hpp"
#include "tiki/container/linked_list.hpp"
#include "tiki/io/file_stream.hpp"

namespace tiki
{
	class GamebuildFileSystem : public FileSystem
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( GamebuildFileSystem );

	public:

		bool				create( const Path& assetBuildPath, uint maxStreamCount = 4u );
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

		Path				m_assetBuildPath;
		GamebuildFileList	m_files;

		Array< FileStream >	m_fileStreams;

	};
}
