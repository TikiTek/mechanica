#include "tiki/converter_base/conversion_parameters.hpp"

#include "tiki/base/string_tools.hpp"

namespace tiki
{
	void ConversionParameters::copyFrom( const ConversionParameters& parameters )
	{
		for( uint i = 0u; i < parameters.m_parameters.getCount(); ++i )
		{
			const KeyValuePair< DynamicString, DynamicString >& kvp = parameters.m_parameters.getPairAt( i );
			m_parameters.set( kvp.key, kvp.value );
		}
	}

	void ConversionParameters::addParameter( const DynamicString& key, const DynamicString& value )
	{
		m_parameters.set( key, value );
	}

	DynamicString ConversionParameters::getString( const DynamicString& key ) const
	{
		DynamicString value;
		if( !getArgument( value, key ) )
		{
			TIKI_TRACE_ERROR( "[converter] parameters '%s' not found.\n", key.cStr() );
			return DynamicString();
		}

		return value;
	}

	float ConversionParameters::getFloat( const DynamicString& key ) const
	{
		DynamicString value;
		if( !getArgument( value, key ) )
		{
			TIKI_TRACE_ERROR( "[converter] parameters '%s' not found.\n", key.cStr() );
			return 0.0f;
		}

		return string_tools::parseFloat32( value.cStr() );
	}

	int ConversionParameters::getInt( const DynamicString& key ) const
	{
		DynamicString value;
		if( !getArgument( value, key ) )
		{
			TIKI_TRACE_ERROR( "[converter] parameters '%s' not found.\n", key.cStr() );
			return 0;
		}

		return string_tools::parseSInt32( value.cStr() );
	}

	DynamicString ConversionParameters::getOptionalString( const DynamicString& key, const DynamicString& defaultValue ) const
	{
		DynamicString value;
		if ( getArgument( value, key ) )
		{
			return value;
		}

		return defaultValue;
	}

	float ConversionParameters::getOptionalFloat( const DynamicString& key, float defaultValue ) const
	{
		DynamicString value;
		if ( getArgument( value, key ) )
		{
			return string_tools::parseFloat32( value.cStr() );
		}

		return defaultValue;
	}

	int ConversionParameters::getOptionalInt( const DynamicString& key, int defaultValue ) const
	{
		DynamicString value;
		if ( getArgument( value, key ) )
		{
			return string_tools::parseSInt32( value.cStr() );
		}

		return defaultValue;
	}

	bool ConversionParameters::getArgument( DynamicString& value, const DynamicString& key ) const
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

	bool ConversionParameters::getBool( const DynamicString& key ) const
	{
		DynamicString value;
		TIKI_VERIFY( getArgument( value, key ) );

		return value.toLower() == "true";
	}

	bool ConversionParameters::getOptionalBool( const DynamicString& key, bool defaultValue ) const
	{
		DynamicString value;
		if ( getArgument( value, key ) )
		{
			return value.toLower() == "true";
		}

		return defaultValue;
	}
}
