
#include "tiki/converterbase/conversionparameters.hpp"

#include "tiki/base/stringparse.hpp"

namespace tiki
{
	Map< string, string >& ConversionArguments::getMap()
	{
		return m_arguments;
	}

	const Map< string, string >& ConversionArguments::getMap() const
	{
		return m_arguments;
	}

	string ConversionArguments::getString( const string& key ) const
	{
		string value;
		TIKI_VERIFY( getArgument( value, key ) );

		return value;
	}

	float ConversionArguments::getFloat( const string& key ) const
	{
		string value;
		TIKI_VERIFY( getArgument( value, key ) );

		return ParseString::parseSingle( value.cStr() );
	}

	int ConversionArguments::getInt( const string& key ) const
	{
		string value;
		TIKI_VERIFY( getArgument( value, key ) );

		return ParseString::parseInt32( value.cStr() );
	}

	string ConversionArguments::getOptionalString( const string& key, const string& defaultValue ) const
	{
		string value;
		if ( getArgument( value, key ) )
		{
			return value;
		}

		return defaultValue;
	}

	float ConversionArguments::getOptionalFloat( const string& key, float defaultValue ) const
	{
		string value;
		if ( getArgument( value, key ) )
		{
			return ParseString::parseSingle( value.cStr() );
		}

		return defaultValue;
	}

	int ConversionArguments::getOptionalInt( const string& key, int defaultValue ) const
	{
		string value;
		if ( getArgument( value, key ) )
		{
			return ParseString::parseInt32( value.cStr() );
		}

		return defaultValue;
	}

	bool ConversionArguments::getArgument( string& value, const string& key ) const
	{
		if ( m_arguments.findValue( &value, key ) )
		{
			return true;
		}
		else
		{
			value = "";
		}

		return false;
	}

	bool ConversionArguments::getBool( const string& key ) const
	{
		string value;
		TIKI_VERIFY( getArgument( value, key ) );

		return value.toLower() == "true";
	}

	bool ConversionArguments::getOptionalBool( const string& key, bool defaultValue ) const
	{
		string value;
		if ( getArgument( value, key ) )
		{
			return value.toLower() == "true";
		}

		return defaultValue;
	}

}
