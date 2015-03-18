
#include "tiki/webinterface/url.hpp"

namespace tiki
{
	Url::Url()
	{
		clear();
	}

	Url::~Url()
	{
	}

	bool Url::createFromString( cstring pString )
	{
		clear();
		m_urlString = pString;

		m_protocolLength = m_urlString.indexOf( ':' );
		if ( m_protocolLength <= 0 || m_urlString[ m_protocolLength ] != ':' )
		{
			TIKI_TRACE_WARNING( "[WebInterface][Url] Url has no valid format: Protocol not found.\n" );

			clear();
			return false;
		}

		m_protocolEndIndex = m_urlString.lastIndexOf( '/', m_protocolLength + 3 ) + 1;
		m_protocolEndIndex = TIKI_MAX( m_protocolEndIndex, m_urlString.lastIndexOf( ':', m_protocolLength + 3 ) + 1 );
		if ( m_protocolEndIndex < m_protocolLength )
		{
			TIKI_TRACE_WARNING( "[WebInterface][Url] Url has no valid format: Protocol end not found.\n" );

			clear();
			return false;
		}
		
		int pathBeginIndex = m_urlString.indexOf( '/', m_protocolEndIndex );
		int pathEndIndex = m_urlString.indexOf( '?', pathBeginIndex );
		if ( pathBeginIndex < 0 )
		{
			pathBeginIndex = ( pathEndIndex < 0 ? (int)m_urlString.getLength() : pathEndIndex );
		}

		const int authEndIndex = m_urlString.lastIndexOf( '@', pathBeginIndex );
		if ( authEndIndex > 0 )
		{
			int passwordBeginIndex = m_urlString.lastIndexOf( ':', authEndIndex ) + 1;
			if ( passwordBeginIndex > m_protocolEndIndex )
			{
				m_usernameLength = (passwordBeginIndex - m_protocolEndIndex) - 1;
				m_passwordLength = authEndIndex - passwordBeginIndex;
			}
			else
			{
				m_usernameLength = authEndIndex - m_protocolEndIndex;
			}

			m_authDataEndIndex = authEndIndex + 1;
		}
		else
		{
			m_authDataEndIndex = m_protocolEndIndex;
		}

		int portBeginIndex = m_urlString.lastIndexOf( ':', pathBeginIndex ) + 1;
		if ( portBeginIndex >= m_authDataEndIndex )
		{
			m_portLength = pathBeginIndex - portBeginIndex;
			m_domainLength = (portBeginIndex - m_authDataEndIndex) - 1;
			m_portEndIndex = portBeginIndex + m_portLength;
		}
		else
		{
			m_domainLength = pathBeginIndex - m_authDataEndIndex;
			m_portEndIndex = m_authDataEndIndex + m_domainLength;
		}
		
		int fragmentIndex = m_urlString.lastIndexOf( '#' ) + 1;
		if ( fragmentIndex > 0 )
		{
			m_fragmentLength = (int)m_urlString.getLength() - fragmentIndex;
		}

		if ( pathEndIndex < 0 )
		{
			pathEndIndex = (int)m_urlString.getLength();
		}
		else
		{
			const int stringEndIndex = (int)m_urlString.getLength() - ( m_fragmentLength > 0 ? m_fragmentLength + 1 : 0 );

			int currentIndex = pathEndIndex + 1;
			int endIndex = 0u;

			do 
			{
				endIndex = m_urlString.indexOf( '&', currentIndex );
				if ( endIndex < 0 )
				{
					endIndex = stringEndIndex;
				}

				int equalsIndex = m_urlString.indexOf( '=', currentIndex );
				if ( equalsIndex < 0 )
				{
					equalsIndex = endIndex - 1;
				}

				const string key = m_urlString.subString( currentIndex, equalsIndex - currentIndex );
				const string value = m_urlString.subString( equalsIndex + 1, endIndex - (equalsIndex + 1) );

				m_arguments.set(key, value);
				currentIndex = endIndex + 1;
			}
			while ( endIndex < stringEndIndex );
		}
		m_pathLength = pathEndIndex - pathBeginIndex;
		
		return true;
	}

	void Url::clear()
	{
		m_urlString = "";

		m_protocolLength	= 0u;
		m_protocolEndIndex	= 0u;
		m_usernameLength	= 0u;
		m_passwordLength	= 0u;
		m_authDataEndIndex	= 0u;
		m_domainLength		= 0u;
		m_portLength		= 0u;
		m_portEndIndex		= 0u;
		m_pathLength		= 0u;
		m_fragmentLength	= 0u;

		m_arguments.clear();
	}

	bool Url::isValid() const
	{
		return !m_urlString.isEmpty() && m_protocolLength > 0 && m_domainLength > 0;
	}

	bool Url::match( const Url& other ) const
	{
		const string thisUrl = getUrlWithoutQuery();
		const string otherUrl = other.getUrlWithoutQuery();

		int indexOfStart = thisUrl.indexOf( '*' );
		if ( indexOfStart < 0 )
		{
			return thisUrl == otherUrl;
		}
		else
		{
			int thisIndex = 0u;
			int otherIndex = 0u;
			int length = indexOfStart;

			while (true)
			{
				const string thisPart = thisUrl.subString( thisIndex, length );
				const string otherPart = thisUrl.subString( thisIndex, length );

				if ( thisPart != otherPart )
				{
					return false;
				}

				thisIndex += length + 1;
				otherIndex += length;

				if ( thisIndex >= (int)thisUrl.getLength() )
				{
					break;
				}

				const char nextChar = thisUrl[thisIndex];
				otherIndex = otherUrl.indexOf( nextChar, otherIndex );
				if ( otherIndex < 0 )
				{
					otherIndex = otherUrl.getLength();
				}

				indexOfStart = thisUrl.indexOf( '*', thisIndex );
				if ( indexOfStart < 0 )
				{
					length = thisUrl.getLength() - thisIndex;
				}
				else
				{
					length = indexOfStart - thisIndex;
				}
			}			

			return true;
		}
	}

	string Url::getProtocol() const
	{
		return m_urlString.subString( 0u, m_protocolLength );
	}

	void Url::setProtokol( const string& protocol )
	{
		m_urlString = m_urlString.remove( 0u, m_protocolLength );
		m_urlString = m_urlString.insert( protocol, 0u );

		const int lengthDiff = (int)protocol.getLength() - m_protocolLength;
		m_protocolEndIndex += lengthDiff;
		m_authDataEndIndex += lengthDiff;
		m_portEndIndex += lengthDiff;
	}

	string Url::getUsername() const
	{
		return m_urlString.subString( m_protocolEndIndex, m_usernameLength );
	}

	string Url::getPassword() const
	{
		return m_urlString.subString( m_protocolEndIndex + m_usernameLength + 1u, m_passwordLength );
	}

	void Url::setAuthData( const string& username, const string& password )
	{
		const int authLength = (m_authDataEndIndex - m_protocolEndIndex);

		if ( m_protocolEndIndex != m_authDataEndIndex )
		{
			m_urlString = m_urlString.remove( m_protocolEndIndex, authLength );
		}

		const string authString = formatString( "%s:%s@", username.cStr(), password.cStr() );
		m_urlString = m_urlString.insert( authString, m_protocolEndIndex );

		const int lengthDiff = (int)authString.getLength() - authLength;
		m_authDataEndIndex += lengthDiff;
		m_portEndIndex += lengthDiff;

		m_usernameLength = (int)username.getLength();
		m_passwordLength = (int)password.getLength();
	}

	string Url::getDomain() const
	{
		return m_urlString.subString( m_authDataEndIndex, m_domainLength );
	}

	void Url::setDomain( const string& domain )
	{
		m_urlString = m_urlString.remove( m_authDataEndIndex, m_domainLength );
		m_urlString = m_urlString.insert( domain, m_authDataEndIndex );

		const int lengthDiff = (int)domain.getLength() - m_domainLength;
		m_portEndIndex += lengthDiff;

		m_domainLength = (int)domain.getLength();
	}

	string Url::getPort() const
	{
		if ( m_portLength > 0 )
		{
			return m_urlString.subString( m_authDataEndIndex + m_domainLength + 1u, m_portLength );
		}

		return nullptr;
	}

	void Url::setPort( const string& port )
	{
		const int portIndex = m_authDataEndIndex + m_domainLength;

		if ( m_portLength > 0 )
		{
			m_urlString = m_urlString.remove( portIndex, m_portLength + 1u );
		}

		const string portString = formatString( ":%s", port.cStr() );
		m_urlString = m_urlString.insert( portString, portIndex );

		int lengthDiff = port.getLength() - m_portLength;
		m_portLength = (int)port.getLength();
		m_portEndIndex += lengthDiff;
	}

	string Url::getPath() const
	{
		return m_urlString.subString( m_portEndIndex, m_pathLength );
	}

	void Url::setPath( const string& path )
	{
		m_urlString = m_urlString.remove( m_portEndIndex, m_pathLength );

		const string pathString = ( path[ 0u ] != '/' ? "/" : "" ) + path;
		m_urlString = m_urlString.insert( pathString, m_portEndIndex );

		m_pathLength = pathString.getLength();
	}

	string Url::getQueryValue( const string& key ) const
	{
		string value;
		m_arguments.findValue( &value, key );

		return value;
	}

	void Url::setQueryValue( const string& key, const string& value )
	{
		string oldValue;
		if ( m_arguments.findValue( &oldValue, key ) )
		{
			const string oldPair = formatString( "%s=%s", key.cStr(), oldValue.cStr() );
			const string newPair = formatString( "%s=%s", key.cStr(), value.cStr() );
			m_urlString = m_urlString.replace( oldPair, newPair );
		}
		else
		{
			const int fragmentIndex = (int)m_urlString.getLength() - m_fragmentLength - 1;
			const string newPair = formatString( "%s%s=%s", ( m_arguments.isEmpty() ? "?" : "&" ), key.cStr(), value.cStr() );
			m_urlString = m_urlString.insert( newPair, fragmentIndex );
		}

		m_arguments.set( key, value );
	}

	string Url::getFragment() const
	{
		const int fragmentIndex = m_urlString.getLength() - m_fragmentLength;
		return m_urlString.subString( fragmentIndex, m_fragmentLength );
	}

	void Url::setFragment( const string& fragment )
	{
		if ( m_fragmentLength > 0 )
		{
			const int fragmentIndex = m_urlString.getLength() - m_fragmentLength - 1;
			m_urlString = m_urlString.remove( fragmentIndex, m_fragmentLength + 1u );
		}

		m_urlString += formatString( "#%s", fragment.cStr() );

		m_fragmentLength = (int)fragment.getLength();
	}

	string Url::getUrl() const
	{
		return m_urlString;
	}

	string Url::getUrlWithoutQuery() const
	{
		return m_urlString.subString( 0u, m_portEndIndex + m_pathLength );
	}
}
