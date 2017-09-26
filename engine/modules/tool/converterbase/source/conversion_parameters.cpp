#include "tiki/converterbase/conversion_parameters.hpp"

#include "tiki/base/string_tools.hpp"

namespace tiki
{
	Map< string, string >& ConversionParameters::getMap()
	{
		return m_parameters;
	}

	const Map< string, string >& ConversionParameters::getMap() const
	{
		return m_parameters;
	}

	string ConversionParameters::getString( const string& key ) const
	{
		string value;
		TIKI_VERIFY( getArgument( value, key ) );

		return value;
	}

	float ConversionParameters::getFloat( const string& key ) const
	{
		string value;
		TIKI_VERIFY( getArgument( value, key ) );

		return string_tools::parseFloat32( value.cStr() );
	}

	int ConversionParameters::getInt( const string& key ) const
	{
		string value;
		TIKI_VERIFY( getArgument( value, key ) );

		return string_tools::parseSInt32( value.cStr() );
	}

	string ConversionParameters::getOptionalString( const string& key, const string& defaultValue ) const
	{
		string value;
		if ( getArgument( value, key ) )
		{
			return value;
		}

		return defaultValue;
	}

	float ConversionParameters::getOptionalFloat( const string& key, float defaultValue ) const
	{
		string value;
		if ( getArgument( value, key ) )
		{
			return string_tools::parseFloat32( value.cStr() );
		}

		return defaultValue;
	}

	int ConversionParameters::getOptionalInt( const string& key, int defaultValue ) const
	{
		string value;
		if ( getArgument( value, key ) )
		{
			return string_tools::parseSInt32( value.cStr() );
		}

		return defaultValue;
	}

	bool ConversionParameters::getArgument( string& value, const string& key ) const
	{
		if ( m_parameters.findValue( &value, key ) )
		{
			return true;
		}
		else
		{
			value = "";
		}

		return false;
	}

	bool ConversionParameters::getBool( const string& key ) const
	{
		string value;
		TIKI_VERIFY( getArgument( value, key ) );

		return value.toLower() == "true";
	}

	bool ConversionParameters::getOptionalBool( const string& key, bool defaultValue ) const
	{
		string value;
		if ( getArgument( value, key ) )
		{
			return value.toLower() == "true";
		}

		return defaultValue;
	}
}
