#pragma once
#ifndef TIKI_CONVERSION_PARAMETERS_HPP_INCLUDED
#define TIKI_CONVERSION_PARAMETERS_HPP_INCLUDED

#include "tiki/base/dynamic_string.hpp"
#include "tiki/container/map.hpp"

namespace tiki
{
	class ConversionParameters
	{
	public:

		Map< string, string >&			getMap();
		const Map< string, string >&	getMap() const;

		string							getString( const string& key ) const;
		float							getFloat( const string& key ) const;
		int								getInt( const string& key ) const;
		bool							getBool( const string& key ) const;

		string							getOptionalString( const string& key, const string& defaultValue ) const;
		float							getOptionalFloat( const string& key, float defaultValue ) const;
		int								getOptionalInt( const string& key, int defaultValue ) const;
		bool							getOptionalBool( const string& key, bool defaultValue ) const;

	private:

		Map< string, string >			m_parameters;

		bool							getArgument( string& value, const string& key ) const;
	};
}

#endif // TIKI_CONVERSION_PARAMETERS_HPP_INCLUDED
