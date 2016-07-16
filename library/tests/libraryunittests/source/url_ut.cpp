
#include "tiki/unittest/unittest.hpp"

#include "tiki/net/url.hpp"

namespace tiki
{
	static bool createUrlHttpFull( Url& url, Map<string, string>& arguments )
	{
		if ( !url.createFromString( "http://hans:geheim@example.org:80/demo/example.cgi?land=de&stadt=aa#geschichte" ) )
		{
			return false;
		}

		arguments.set("land", "de");
		arguments.set("stadt", "aa");

		return true;
	}

	static bool checkUrlIntegrity( const Url& url, const char* pProtocol, const char* pUsername, const char* pPassword, const char* pDomain, const char* pPort, const char* pPath, const Map<string, string>* pArguments, const char* pFragment )
	{
		bool result = true;

		if ( !pProtocol && url.getProtocol() != pProtocol )
		{
			TIKI_TRACE_WARNING( "[Url][checkUrlIntegrity] protocol check failed!\n" );
			result = false;
		}

		if ( pUsername && url.getUsername() != pUsername )
		{
			TIKI_TRACE_WARNING( "[Url][checkUrlIntegrity] username check failed!\n" );
			result = false;
		}

		if ( pPassword && url.getPassword() != pPassword )
		{
			TIKI_TRACE_WARNING( "[Url][checkUrlIntegrity] password check failed!\n" );
			result = false;
		}

		if ( pDomain && url.getDomain() != pDomain )
		{
			TIKI_TRACE_WARNING( "[Url][checkUrlIntegrity] domain check failed!\n" );
			result = false;
		}

		if ( pPort && url.getPort() != pPort )
		{
			TIKI_TRACE_WARNING( "[Url][checkUrlIntegrity] port check failed!\n" );
			result = false;
		}

		if ( pPath && url.getPath() != pPath )
		{
			TIKI_TRACE_WARNING( "[Url][checkUrlIntegrity] path check failed!\n" );
			result = false;
		}

		if ( pArguments )
		{			
			for (uint i = 0u; i < pArguments->getCount(); ++i)
			{
				const Map<string, string>::Pair& pair = pArguments->getPairAt( i );

				string value = url.getQueryValue( pair.key );
				if ( value != pair.value )
				{
					TIKI_TRACE_WARNING( "[Url][checkUrlIntegrity] argument check failed!\n" );
					result = false;
				}
			}
		}

		if ( pFragment && url.getFragment() != pFragment )
		{
			TIKI_TRACE_WARNING( "[Url][checkUrlIntegrity] fragment check failed!\n" );
			result = false;
		}

		return result;
	}

	TIKI_BEGIN_UNITTEST( Url );

	TIKI_ADD_TEST( UrlCreateAndCheckHttpFull )
	{
		Url url;		
		Map<string, string> arguments;
		TIKI_UT_CHECK( createUrlHttpFull( url, arguments ) );
		TIKI_UT_CHECK( checkUrlIntegrity( url, "http", "hans", "geheim", "example.org", "80", "/demo/example.cgi", &arguments, "geschichte" ) );
	}

	TIKI_ADD_TEST( UrlCheckHttpMin )
	{
		Url url;		
		TIKI_UT_CHECK( url.createFromString( "http://example.org" ) );
		TIKI_UT_CHECK( checkUrlIntegrity( url, "http", "", "", "example.org", "", "", nullptr, "" ) );
	}

	TIKI_ADD_TEST( UrlCheckHttpPath )
	{
		Url url;		
		TIKI_UT_CHECK( url.createFromString( "http://example.org/path/to/file.html" ) );
		TIKI_UT_CHECK( checkUrlIntegrity( url, "http", "", "", "example.org", "", "/path/to/file.html", nullptr, "" ) );
	}

	TIKI_ADD_TEST( UrlCheckMailto )
	{
		Url url;		
		TIKI_UT_CHECK( url.createFromString( "mailto:max.mustermann@example.org" ) );
		TIKI_UT_CHECK( checkUrlIntegrity( url, "mailto", "max.mustermann", "", "example.org", "", "", nullptr, "" ) );
	}

	TIKI_ADD_TEST( UrlSetProtocol )
	{
		Url url;		
		Map<string, string> arguments;
		TIKI_UT_CHECK( createUrlHttpFull( url, arguments ) );

		url.setProtokol( "ftp " );

		TIKI_UT_CHECK( checkUrlIntegrity( url, "ftp", "hans", "geheim", "example.org", "80", "/demo/example.cgi", &arguments, "geschichte" ) );
	}

	TIKI_ADD_TEST( UrlSetAuthData )
	{
		Url url;		
		Map<string, string> arguments;
		TIKI_UT_CHECK( createUrlHttpFull( url, arguments ) );

		url.setAuthData( "max.mustermann", "abc123" );

		TIKI_UT_CHECK( checkUrlIntegrity( url, "ftp", "max.mustermann", "abc123", "example.org", "80", "/demo/example.cgi", &arguments, "geschichte" ) );
	}

	TIKI_ADD_TEST( UrlSetDomain )
	{
		Url url;		
		Map<string, string> arguments;
		TIKI_UT_CHECK( createUrlHttpFull( url, arguments ) );

		url.setDomain( "www.example.org" );

		TIKI_UT_CHECK( checkUrlIntegrity( url, "http", "hans", "geheim", "www.example.org", "80", "/demo/example.cgi", &arguments, "geschichte" ) );
	}

	TIKI_ADD_TEST( UrlSetPort )
	{
		Url url;		
		Map<string, string> arguments;
		TIKI_UT_CHECK( createUrlHttpFull( url, arguments ) );

		url.setPort( "8080" );

		TIKI_UT_CHECK( checkUrlIntegrity( url, "http", "hans", "geheim", "example.org", "8080", "/demo/example.cgi", &arguments, "geschichte" ) );
	}

	TIKI_ADD_TEST( UrlSetPath )
	{
		Url url;		
		Map<string, string> arguments;
		TIKI_UT_CHECK( createUrlHttpFull( url, arguments ) );

		url.setPath( "path/to/file.html" );

		TIKI_UT_CHECK( checkUrlIntegrity( url, "http", "hans", "geheim", "example.org", "80", "/path/to/file.html", &arguments, "geschichte" ) );
	}

	TIKI_ADD_TEST( UrlSetNewQuery )
	{
		Url url;		
		Map<string, string> arguments;
		TIKI_UT_CHECK( createUrlHttpFull( url, arguments ) );

		arguments.set( "strasse", "bb" );
		url.setQueryValue( "strasse", "bb" );

		TIKI_UT_CHECK( checkUrlIntegrity( url, "http", "hans", "geheim", "example.org", "80", "/demo/example.cgi", &arguments, "geschichte" ) );
	}

	TIKI_ADD_TEST( UrlSetExistingQuery )
	{
		Url url;		
		Map<string, string> arguments;
		TIKI_UT_CHECK( createUrlHttpFull( url, arguments ) );

		arguments.set( "stadt", "cc" );
		url.setQueryValue( "stadt", "cc" );

		TIKI_UT_CHECK( checkUrlIntegrity( url, "http", "hans", "geheim", "example.org", "80", "/demo/example.cgi", &arguments, "geschichte" ) );
	}

	TIKI_ADD_TEST( UrlSetFragment )
	{
		Url url;		
		Map<string, string> arguments;
		TIKI_UT_CHECK( createUrlHttpFull( url, arguments ) );

		url.setFragment( "history" );

		TIKI_UT_CHECK( checkUrlIntegrity( url, "http", "hans", "geheim", "example.org", "80", "/demo/example.cgi", &arguments, "history" ) );
	}

	TIKI_ADD_TEST( UrlGetPathWithoutQuery )
	{
		Url url;		
		Map<string, string> arguments;
		TIKI_UT_CHECK( createUrlHttpFull( url, arguments ) );

		TIKI_UT_CHECK( url.getUrlWithoutQuery() == "http://hans:geheim@example.org:80/demo/example.cgi" );
	}

	TIKI_ADD_TEST( UrlMatch1 )
	{
		Url url1;
		Url url2;
		Map<string, string> arguments;
		TIKI_UT_CHECK( createUrlHttpFull( url1, arguments ) );
		url2 = url1;

		url1.setPath( "/*/example.cgi" );

		TIKI_UT_CHECK( url1.match( url2 ) );
	}

	TIKI_ADD_TEST( UrlMatch2 )
	{
		Url url1;
		Url url2;
		Map<string, string> arguments;
		TIKI_UT_CHECK( createUrlHttpFull( url1, arguments ) );
		url2 = url1;

		url1.setPath( "/*/*.cgi" );

		TIKI_UT_CHECK( url1.match( url2 ) );
	}

	TIKI_ADD_TEST( UrlMatch3 )
	{
		Url url1;
		Url url2;
		Map<string, string> arguments;
		TIKI_UT_CHECK( createUrlHttpFull( url1, arguments ) );
		url2 = url1;

		url1.setPath( "/*/*.*" );

		TIKI_UT_CHECK( url1.match( url2 ) );
	}
}