#pragma once

#include "tiki/base/types.hpp"

namespace tiki
{
	class Path
	{
	public:

									Path();
		explicit					Path( const char* pCompletePath );
									Path( const char* pPath1, const char* pPath2 );
									Path( const Path& path1, const char* pPath2 );
									Path( const Path& path1, const Path& path2 );

		bool						isEmpty() const;

		void						clear();

		void						setCompletePath( const char* pPath );
		void						setCombinedPath( const char* pPath1, const char* pPath2 );
		void						setCombinedPath( const Path& path1, const char* pPath2 );
		void						setCombinedPath( const Path& path1, const Path& path2 );
		void						setFilenameWithExtension( const char* pFilename );
		void						setPrefix( const char* pPrefix );
		void						setDirectory( const char* pDirectory );
		void						setBasename( const char* pBasename );
		void						setExtension( const char* pExtension );

		bool						pop();
		void						push( const char* pName );
		bool						push( const Path& path );

		const char*					getPrefix() const { return m_prefix; }
		const char*					getDirectory() const { return m_directory; }
		const char*					getBasename() const { return m_basename; }
		const char*					getExtension() const { return m_extension; }

		const char*					getDirectoryWithPrefix() const;
		const char*					getFilenameWithExtension() const;
		const char*					getCompletePath() const;

		bool						operator==( const Path& rhs ) const;
		bool						operator!=( const Path& rhs ) const;

	private:

		static constexpr uintreg	MaxPathLength		= TIKI_MAX_PATH;
		static constexpr uintreg	MaxPrefixLength		= 16u;
		static constexpr uintreg	MaxDirectoryLength	= TIKI_MAX_PATH;
		static constexpr uintreg	MaxFileNameLength	= TIKI_MAX_PATH / 2;
		static constexpr uintreg	MaxExtensionLength	= TIKI_MAX_PATH / 4;

		enum class BufferState
		{
			Invalid,
			DirectoryWithPrefix,
			FilenameWithExtension,
			CompletePath
		};

		char						m_prefix[ MaxPathLength ];
		char						m_directory[ MaxDirectoryLength ];
		char						m_basename[ MaxFileNameLength ];
		char						m_extension[ MaxExtensionLength ];

		mutable BufferState			m_bufferState;
		mutable char				m_buffer[ MaxPathLength ];

		void						buildPath( BufferState targetState ) const;
	};
}
