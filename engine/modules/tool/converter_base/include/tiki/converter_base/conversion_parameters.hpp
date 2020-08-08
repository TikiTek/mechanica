#pragma once

#include "tiki/base/dynamic_string.hpp"
#include "tiki/container/map.hpp"

namespace tiki
{
	class ConversionParameters
	{
	public:

		void								copyFrom( const ConversionParameters& parameters );
		void								addParameter( const DynamicString& key, const DynamicString& value );

		DynamicString						getString( const DynamicString& key ) const;
		float								getFloat( const DynamicString& key ) const;
		int									getInt( const DynamicString& key ) const;
		bool								getBool( const DynamicString& key ) const;

		DynamicString						getOptionalString( const DynamicString& key, const DynamicString& defaultValue ) const;
		float								getOptionalFloat( const DynamicString& key, float defaultValue ) const;
		int									getOptionalInt( const DynamicString& key, int defaultValue ) const;
		bool								getOptionalBool( const DynamicString& key, bool defaultValue ) const;

	private:

		Map< DynamicString, DynamicString >	m_parameters;

		bool								getArgument( DynamicString& value, const DynamicString& key ) const;
	};
}
