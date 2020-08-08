#include "tiki/base/path.hpp"

#include "tiki/base/string.hpp"

namespace tiki
{
	Path::Path()
	{
		clear();
	}

	Path::Path( const char* pCompletePath )
	{
		setCompletePath( pCompletePath );
	}

	Path::Path( const char* pPath1, const char* pPath2 )
	{
		setCombinedPath( pPath1, pPath2 );
	}

	Path::Path( const Path& path1, const char* pPath2 )
	{
		setCombinedPath( path1, pPath2 );
	}

	Path::Path( const Path& path1, const Path& path2 )
	{
		setCombinedPath( path1, path2 );
	}

	bool Path::isEmpty() const
	{
		return isStringEmpty( m_prefix ) &&
			isStringEmpty( m_directory ) &&
			isStringEmpty( m_basename ) &&
			isStringEmpty( m_extension );
	}

	void Path::clear()
	{
		m_bufferState		= BufferState::Invalid;
		m_buffer[ 0u ]		= '\0';
		m_prefix[ 0u ]		= '\0';
		m_directory[ 0u ]	= '\0';
		m_basename[ 0u ]	= '\0';
		m_extension[ 0u ]	= '\0';
	}

	void Path::setCompletePath( const char* pPath )
	{
		m_bufferState		= BufferState::Invalid;
		m_buffer[ 0u ]		= '\0';
		m_prefix[ 0u ]		= '\0';
		m_directory[ 0u ]	= '\0';
		m_basename[ 0u ]	= '\0';
		m_extension[ 0u ]	= '\0';

		const uintreg pathLength = getStringSize( pPath );

		char buffer[ MaxPathLength ];
		copyString( buffer, sizeof( buffer ), pPath );
		stringReplace( buffer, '\\', '/' );

		uintreg currentIndex = 0u;
		const uintreg prefixEnd = stringIndexOf( buffer, ':' );
		if( prefixEnd != TIKI_SIZE_T_MAX )
		{
			const uintreg prefixSize = TIKI_MIN( prefixEnd + 2u, sizeof( m_prefix ) );
			copyString( m_prefix, prefixSize, buffer );

			currentIndex = prefixEnd + 1u;
		}

		if( buffer[ currentIndex ] == '/' )
		{
			currentIndex++;
		}

		const uintreg directoryEnd = stringLastIndexOf( buffer, '/' );
		if( directoryEnd != TIKI_SIZE_T_MAX && directoryEnd != currentIndex )
		{
			const uintreg directorySize = TIKI_MIN( (directoryEnd - currentIndex) + 1u, sizeof( m_directory ) );
			copyString( m_directory, directorySize, buffer + currentIndex );

			currentIndex = directoryEnd + 1u;
		}

		setFilenameWithExtension( buffer + currentIndex );
	}

	void Path::setCombinedPath( const char* pPath1, const char* pPath2 )
	{
		setCompletePath( pPath1 );

		Path path2;
		path2.setCompletePath( pPath2 );
		push( path2 );
	}

	void Path::setCombinedPath( const Path& path1, const char* pPath2 )
	{
		*this = path1;
		push( pPath2 );
	}

	void Path::setCombinedPath( const Path& path1, const Path& path2 )
	{
		*this = path1;
		push( path2 );
	}

	void Path::setFilenameWithExtension( const char* pFilename )
	{
		const uintreg filenameSize = getStringSize( pFilename );

		uintreg filenameEnd = stringLastIndexOf( pFilename, '.' );
		if( filenameEnd == TIKI_SIZE_T_MAX )
		{
			filenameEnd = filenameSize + 1u;
		}

		const uintreg fileNameSize = TIKI_MIN( filenameEnd + 1u, sizeof( m_basename ) );
		copyString( m_basename, fileNameSize, pFilename );

		if( filenameEnd != filenameSize + 1u )
		{
			const uintreg extensionSize = TIKI_MIN( (filenameSize - filenameEnd) + 1u, sizeof( m_extension ) );
			copyString( m_extension, extensionSize, pFilename + filenameEnd );
		}
		else
		{
			m_extension[ 0u ] = '\0';
		}

		m_bufferState = BufferState::Invalid;
	}

	void Path::setPrefix( const char* pPrefix )
	{
		copyString( m_prefix, sizeof( m_prefix ), pPrefix );
		m_bufferState = BufferState::Invalid;
	}

	void Path::setDirectory( const char* pDirectory )
	{
		copyString( m_directory, sizeof( m_directory ), pDirectory );
		m_bufferState = BufferState::Invalid;
	}

	void Path::setBasename( const char* pBasename )
	{
		copyString( m_basename, sizeof( m_basename ), pBasename );
		m_bufferState = BufferState::Invalid;
	}

	void Path::setExtension( const char* pExtension )
	{
		if( isStringEmpty( pExtension ) )
		{
			m_extension[ 0u ] = '\0';
		}
		else if( pExtension[ 0u ] != '.' )
		{
			m_extension[ 0u ] = '.';
			copyString( m_extension + 1u, sizeof( m_extension ) - 1u, pExtension );
		}
		else
		{
			copyString( m_extension, sizeof( m_extension ), pExtension );
		}

		m_bufferState = BufferState::Invalid;
	}

	bool Path::pop()
	{
		const uintreg index = stringLastIndexOf( m_directory, '/' );
		if( index == TIKI_SIZE_T_MAX )
		{
			return false;
		}

		setFilenameWithExtension( m_directory + index + 1u );
		m_directory[ index ] = '\0';

		m_bufferState = BufferState::Invalid;
		return true;
	}

	void Path::push( const char* pName )
	{
		uintreg index = getStringSize( m_directory );
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
			uintreg index = getStringSize( m_directory );
			if( !isStringEmpty( m_directory ) )
			{
				copyString( m_directory + index, sizeof( m_directory ) - index, "/" );
				index++;
			}

			copyString( m_directory + index, sizeof( m_directory ) - index, getFilenameWithExtension() );
			index += getStringSize( pFilename );
		}

		if( !isStringEmpty( path.getDirectory() ) )
		{
			uintreg index = getStringSize( m_directory );
			if( !isStringEmpty( m_directory ) )
			{
				copyString( m_directory + index, sizeof( m_directory ) - index, "/" );
				index++;
			}

			copyString( m_directory + index, sizeof( m_directory ) - index, path.getDirectory() );
		}

		setBasename( path.getBasename() );
		setExtension( path.getExtension() );

		return true;
	}

	const char* Path::getDirectoryWithPrefix() const
	{
		buildPath( BufferState::DirectoryWithPrefix );
		return m_buffer;
	}

	const char* Path::getFilenameWithExtension() const
	{
		buildPath( BufferState::FilenameWithExtension );
		return m_buffer;
	}

	const char* Path::getCompletePath() const
	{
		buildPath( BufferState::CompletePath );
		return m_buffer;
	}

	bool Path::operator==( const Path& rhs ) const
	{
		return isStringEquals( m_prefix, rhs.m_prefix ) &&
			isStringEquals( m_directory, rhs.m_directory ) &&
			isStringEquals( m_basename, rhs.m_basename ) &&
			isStringEquals( m_extension, rhs.m_extension );
	}

	bool Path::operator!=( const Path& rhs ) const
	{
		return !isStringEquals( m_prefix, rhs.m_prefix ) ||
			!isStringEquals( m_directory, rhs.m_directory ) ||
			!isStringEquals( m_basename, rhs.m_basename ) ||
			!isStringEquals( m_extension, rhs.m_extension );
	}

	void Path::buildPath( BufferState targetState ) const
	{
		if( m_bufferState == targetState )
		{
			return;
		}

		switch( targetState )
		{
		case BufferState::DirectoryWithPrefix:
			{
				char* pBuffer = m_buffer;
				uintreg bufferSize = sizeof( m_buffer );

				uintreg stringSize = copyString( pBuffer, bufferSize, m_prefix );
				pBuffer += stringSize;
				bufferSize -= stringSize;

				if( !isStringEmpty( m_prefix ) )
				{
					stringSize = copyString( pBuffer, bufferSize, "/" );
					pBuffer += stringSize;
					bufferSize -= stringSize;
				}

				stringSize = copyString( pBuffer, bufferSize, m_directory );
				pBuffer += stringSize;
				bufferSize -= stringSize;
			}
			break;

		case BufferState::FilenameWithExtension:
			{
				copyString( m_buffer, sizeof( m_buffer ), m_basename );

				const uintreg bufferSize = getStringSize( m_buffer );
				copyString( m_buffer + bufferSize, sizeof( m_buffer ) - bufferSize, m_extension );
			}
			break;

		case BufferState::CompletePath:
			{
				char* pBuffer = m_buffer;
				uintreg bufferSize = sizeof( m_buffer );

				uintreg stringSize = copyString( pBuffer, bufferSize, m_prefix );
				pBuffer += stringSize;
				bufferSize -= stringSize;

				if( !isStringEmpty( m_prefix ) )
				{
					stringSize = copyString( pBuffer, bufferSize, "/" );
					pBuffer += stringSize;
					bufferSize -= stringSize;
				}

				stringSize = copyString( pBuffer, bufferSize, m_directory );
				pBuffer += stringSize;
				bufferSize -= stringSize;

				if( !isStringEmpty( m_directory ) )
				{
					stringSize = copyString( pBuffer, bufferSize, "/" );
					pBuffer += stringSize;
					bufferSize -= stringSize;
				}

				stringSize = copyString( pBuffer, bufferSize, m_basename );
				pBuffer += stringSize;
				bufferSize -= stringSize;

				copyString( pBuffer, bufferSize, m_extension );
			}
			break;

		default:
			break;
		}

		m_bufferState = targetState;
	}
}
