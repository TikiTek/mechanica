#pragma once
#ifndef TIKI_CONVERSIONPARAMETERS_HPP
#define TIKI_CONVERSIONPARAMETERS_HPP

#include "tiki/base/platform.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/toolbase/list.hpp"

#include <map>

namespace tiki
{
	struct ConversionInputFile
	{
		string			fileName;

		string			typeString;
		crc32			inputType;
	};

	class ConversionArguments
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( ConversionArguments );

	public:

		std::map< string, string >&			getMap();
		const std::map< string, string >&	getMap() const;

		string								getString( const string& key ) const;
		float								getFloat( const string& key ) const;
		int									getInt( const string& key ) const;
		bool								getBool( const string& key ) const;

		string								getOptionalString( const string& key, const string& defaultValue ) const;
		float								getOptionalFloat( const string& key, float defaultValue ) const;
		int									getOptionalInt( const string& key, int defaultValue ) const;
		bool								getOptionalBool( const string& key, bool defaultValue ) const;

	private:

		std::map< string, string >			m_arguments;

		bool								getArgument( string& value, const string& key ) const; 

	};

	struct ConversionParameters
	{
		PlatformType					targetPlatform;

		string							sourceFile;
		string							outputName;

		List< ConversionInputFile >		inputFiles;
		ConversionArguments				arguments;
	};
}

#endif // TIKI_CONVERSIONPARAMETERS_HPP
