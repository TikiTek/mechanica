#include "tiki/converterbase/conversion_parameters.hpp"

#include "tiki/base/string_tools.hpp"

namespace tiki
{
	void ConversionParameters::copyFrom( const ConversionParameters& parameters )
	{
		for( uint i = 0u; i < parameters.m_parameters.getCount(); ++i )
		{
			const KeyValuePair< string, string >& kvp = parameters.m_parameters.getPairAt( i );
			m_parameters.set( kvp.key, kvp.value );
		}
	}

	void ConversionParameters::addParameter( const string& key, const string& value )
	{
		m_parameters.set( key, value );
	}

	string ConversionParameters::getString( const string& key ) const
	{
		string value;
		if( !getArgument( value, key ) )
		{
			TIKI_TRACE_ERROR( "[converter] parameters '%s' not found.\n", key.cStr() );
			return string();
		}

		return value;
	}

	float ConversionParameters::getFloat( const string& key ) const
	{
		string value;
		if( !getArgument( value, key ) )
		{
			TIKI_TRACE_ERROR( "[converter] parameters '%s' not found.\n", key.cStr() );
			return 0.0f;
		}

		return string_tools::parseFloat32( value.cStr() );
	}

	int ConversionParameters::getInt( const string& key ) const
	{
		string value;
		if( !getArgument( value, key ) )
		{
			TIKI_TRACE_ERROR( "[converter] parameters '%s' not found.\n", key.cStr() );
			return 0;
		}

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
			value.clear();
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
