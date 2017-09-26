#include "tiki/base/path.hpp"

#include "tiki/base/string.hpp"

namespace tiki
{
	Path::Path()
	{
		m_bufferState		= BufferState_Invalid;
		m_buffer[ 0u ]		= '\0';
		m_prefix[ 0u ]		= '\0';
		m_directory[ 0u ]	= '\0';
		m_filename[ 0u ]	= '\0';
		m_extension[ 0u ]	= '\0';
	}

	void Path::setCompletePath( const char* pPath )
	{
		m_bufferState		= BufferState_Invalid;
		m_buffer[ 0u ]		= '\0';
		m_prefix[ 0u ]		= '\0';
		m_directory[ 0u ]	= '\0';
		m_filename[ 0u ]	= '\0';
		m_extension[ 0u ]	= '\0';

		const uint pathLength = getStringSize( pPath );

		char buffer[ MaxPathLength ];
		copyString( buffer, sizeof( buffer ), pPath );
		stringReplace( buffer, '\\', '/' );

		uint currentIndex = 0u;
		const uint prefixEnd = stringIndexOf( buffer, ':' );
		if( prefixEnd != TIKI_SIZE_T_MAX )
		{
			const uint prefixSize = TIKI_MIN( prefixEnd + 2u, sizeof( m_prefix ) );
			copyString( m_prefix, prefixSize, buffer );

			currentIndex = prefixEnd + 1u;
		}

		if( buffer[ currentIndex ] == '/' )
		{
			currentIndex++;
		}

		const uint directoryEnd = stringLastIndexOf( buffer, '/' );
		if( directoryEnd != TIKI_SIZE_T_MAX && directoryEnd != currentIndex )
		{
			const uint directorySize = TIKI_MIN( (directoryEnd - currentIndex) + 1u, sizeof( m_directory ) );
			copyString( m_directory, directorySize, buffer + currentIndex );

			currentIndex = directoryEnd + 1u;
		}

		setFilenameWithExtension( buffer + currentIndex );

		//uint filenameEnd = stringLastIndexOf( buffer, '.' );
		//if( filenameEnd == TIKI_SIZE_T_MAX )
		//{
		//	filenameEnd = pathLength + 1u;
		//}

		//const uint fileNameSize = TIKI_MIN( (filenameEnd - currentIndex) + 1u, sizeof( m_filename ) );
		//copyString( m_filename, fileNameSize, buffer + currentIndex );

		//if( filenameEnd != pathLength + 1u )
		//{
		//	const uint extensionSize = TIKI_MIN( (pathLength - filenameEnd) + 1u, sizeof( m_extension ) );
		//	copyString( m_extension, extensionSize, buffer + filenameEnd );
		//}
		//else
		//{
		//	m_extension[ 0u ] = '\0';
		//}
	}

	void Path::setCombinedPath( const char* pPath1, const char* pPath2 )
	{
		setCompletePath( pPath1 );

		Path path2;
		path2.setCompletePath( pPath2 );
		push( path2 );
	}

	void Path::setFilenameWithExtension( const char* pFilename )
	{
		const uint filenameSize = getStringSize( pFilename );

		uint filenameEnd = stringLastIndexOf( pFilename, '.' );
		if( filenameEnd == TIKI_SIZE_T_MAX )
		{
			filenameEnd = filenameSize + 1u;
		}

		const uint fileNameSize = TIKI_MIN( filenameEnd + 1u, sizeof( m_filename ) );
		copyString( m_filename, fileNameSize, pFilename );

		if( filenameEnd != filenameSize + 1u )
		{
			const uint extensionSize = TIKI_MIN( (filenameSize - filenameEnd) + 1u, sizeof( m_extension ) );
			copyString( m_extension, extensionSize, pFilename + filenameEnd );
		}
		else
		{
			m_extension[ 0u ] = '\0';
		}
	}

	void Path::setPrefix( const char* pPrefix )
	{
		copyString( m_prefix, sizeof( m_prefix ), pPrefix );
	}

	void Path::setDirectory( const char* pDirectory )
	{
		copyString( m_directory, sizeof( m_directory ), pDirectory );
	}

	void Path::setFilename( const char* pFilename )
	{
		copyString( m_filename, sizeof( m_filename ), pFilename );
	}

	void Path::setExtension( const char* pExtension )
	{
		copyString( m_extension, sizeof( m_extension ), pExtension );
	}

	bool Path::pop()
	{
		const uint index = stringLastIndexOf( m_directory, '/' );
		if( index == TIKI_SIZE_T_MAX )
		{
			return false;
		}

		setFilenameWithExtension( m_directory + index );
		m_directory[ index ] = '\0';

		return true;
	}

	void Path::push( const char* pName )
	{
		uint index = getStringSize( m_directory );
		copyString( m_directory + index, sizeof( m_directory ) - index, "/" );
		index++;

		copyString( m_directory + index, sizeof( m_directory ) - index, getFilenameWithExtension() );
		setFilenameWithExtension( pName );
	}

	bool Path::push( const Path& path )
	{
		if( !isStringEmpty( path.getPrefix() ) )
		{
			TIKI_TRACE_ERROR( "[path] Can't push path with prefix!\n" );
			return false;
		}

		const char* pFilename = getFilenameWithExtension();
		if( !isStringEmpty( pFilename ) )
		{
			uint index = getStringSize( m_directory );
			copyString( m_directory + index, sizeof( m_directory ) - index, "/" );
			index++;

			copyString( m_directory + index, sizeof( m_directory ) - index, getFilenameWithExtension() );
			index += getStringSize( pFilename );
		}

		if( !isStringEmpty( path.getDirectory() ) )
		{
			uint index = getStringSize( m_directory );
			copyString( m_directory + index, sizeof( m_directory ) - index, "/" );
			index++;

			copyString( m_directory + index, sizeof( m_directory ) - index, path.getDirectory() );
		}

		setFilename( path.getFilename() );
		setExtension( path.getExtension() );

		return true;
	}

	const char* Path::getDirectoryWithPrefix() const
	{
		buildPath( BufferState_DirectoryWithPrefix );
		return m_buffer;
	}

	const char* Path::getFilenameWithExtension() const
	{
		buildPath( BufferState_FilenameWithExtension );
		return m_buffer;
	}

	const char* Path::getCompletePath() const
	{
		buildPath( BufferState_CompletePath );
		return m_buffer;
	}

	void Path::buildPath( BufferState targetState ) const
	{
		if( m_bufferState == targetState )
		{
			return;
		}

		switch( targetState )
		{
		case BufferState_DirectoryWithPrefix:
			break;

		case BufferState_FilenameWithExtension:
			{
				copyString( m_buffer, sizeof( m_buffer ), m_filename );

				const uint bufferSize = getStringSize( m_buffer );
				copyString( m_buffer + bufferSize, sizeof( m_buffer ) - bufferSize, m_extension );
			}
			break;

		case BufferState_CompletePath:
			break;

		default:
			break;
		}

		m_bufferState = targetState;
	}
}
