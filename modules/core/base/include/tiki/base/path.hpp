#pragma once
#ifndef TIKI_PATH_HPP_INCLUDED
#define TIKI_PATH_HPP_INCLUDED

#include "tiki/base/types.hpp"

namespace tiki
{
	class Path
	{
	public:

						Path();

		void			createCompletePath( const char* pPath );
		void			createCombinedPath( const char* pPath1, const char* pPath2 );

		const char*		getPrefix() const { return m_prefix; }
		const char*		getDirectory() const { return m_directory; }
		const char*		getFilename() const { return m_filename; }
		const char*		getExtension() const { return m_extension; }

		void			pop();
		void			push( const char* pName );

		const char*		getCompletePath();

	private:

		enum
		{
			MaxPathLength		= TIKI_MAX_PATH,
			MaxPrefixLength		= 16u,
			MaxDirectoryLength	= TIKI_MAX_PATH,
			MaxFileNameLength	= TIKI_MAX_PATH / 2,
			MaxExtensionLength	= TIKI_MAX_PATH / 4,
		};

		enum BufferState
		{
			BufferState_Invalid,
			BufferState_DirectoryWithPrefix,
			BufferState_FilenameWithExtension,
			BufferState_CompletePath
		};

		BufferState	m_bufferState;
		char		m_buffer[ MaxPathLength ];

		char		m_prefix[ MaxPathLength ];
		char		m_directory[ MaxDirectoryLength ];
		char		m_filename[ MaxFileNameLength ];
		char		m_extension[ MaxExtensionLength ];
	};
}

#endif // TIKI_PATH_HPP_INCLUDED
