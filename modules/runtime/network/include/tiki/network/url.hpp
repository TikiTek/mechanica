#pragma once
#ifndef TIKI_URL_HPP_INCLUDED__
#define TIKI_URL_HPP_INCLUDED__

#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/map.hpp"

namespace tiki
{
	class Url
	{
	public:

		Url();
		~Url();

		bool	createFromString( const char* pString );
		void	clear();

		bool	isValid() const;

		bool	match( const Url& other ) const;

		string	getProtocol() const;
		void	setProtokol( const string& protocol );

		string	getUsername() const;
		string	getPassword() const;
		void	setAuthData( const string& username, const string& password );

		string	getDomain() const;
		void	setDomain( const string& domain );

		string	getPort() const;
		void	setPort( const string& port );

		string	getPath() const;
		void	setPath( const string& path );

		string	getQueryValue( const string& key ) const;
		void	setQueryValue( const string& key, const string& value );

		string	getFragment() const;
		void	setFragment( const string& fragment );

		string	getUrl() const;
		string	getUrlWithoutQuery() const;

	private:

		// URL:
		// protocol:[//][username[:password]@]domain[:port][/path][?var1=value2[&var2=value2]]

		typedef Map<string, string> QueryMap;

		string		m_urlString;

		int			m_protocolLength;
		int			m_protocolEndIndex;
		int			m_usernameLength;
		int			m_passwordLength;
		int			m_authDataEndIndex;
		int			m_domainLength;
		int			m_portLength;
		int			m_portEndIndex;
		int			m_pathLength;
		int			m_fragmentLength;

		QueryMap	m_query;

	};
}

#endif // TIKI_URL_HPP_INCLUDED__
