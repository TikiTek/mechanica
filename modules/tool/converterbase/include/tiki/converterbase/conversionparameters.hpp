#pragma once
#ifndef TIKI_CONVERSIONPARAMETERS_HPP
#define TIKI_CONVERSIONPARAMETERS_HPP

#include "tiki/base/file.hpp"
#include "tiki/base/iopath.hpp"
#include "tiki/base/platform.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/toolbase/list.hpp"

#include <map>

namespace tiki
{
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
		struct InputFile
		{
			string	fileName;
			crc32	typeCrc;
		};

		uint					assetId;

		PlatformType			targetPlatform;
		crc32					typeCrc;

		string					sourceFile;
		string					outputName;

		List< InputFile >		inputFiles;
		ConversionArguments		arguments;
	};

	struct ConversionResult
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( ConversionResult );

	public:

		enum DependencyType
		{
			DependencyType_File,
			DependencyType_Type,
			DependencyType_Converter,

			DependencyType_Count
		};

		struct TraceInfo
		{
			TraceLevel	level;
			string		message;
		};

		struct OutputFile
		{
			fourcc	type;
			string	fileName;
		};

		struct Dependency
		{
			DependencyType	type;
			string			identifier;
			
			int				valueInt;
			string			valueText;
		};

		const List< TraceInfo >& getTraceInfos() const
		{
			return traceInfos;
		}

		const List< OutputFile >& getOutputFiles() const
		{
			return outputFiles;
		}

		const List< Dependency >& getDependencies() const
		{
			return dependencies;
		}

		void addTraceInfo( TraceLevel level, const string& message )
		{
			TraceInfo& traceInfo = traceInfos.add();
			traceInfo.level		= level;
			traceInfo.message	= message;
		}

		void addOutputFile( const string& fileName, fourcc fileType )
		{
			for (uint i = 0u; i < outputFiles.getCount(); ++i)
			{
				OutputFile& file = outputFiles[ i ];

				if ( file.fileName == fileName )
				{
					TIKI_ASSERT( file.type == fileType );
					return;
				}
			}

			OutputFile& file = outputFiles.add();
			file.fileName	= fileName;
			file.type		= fileType;
		}

		void addDependency( DependencyType type, const string& identifier, const string& valueText, int valueInt )
		{
			string id = identifier;
			if ( type == DependencyType_File )
			{
				id			= path::getAbsolutePath( id );
				valueInt	= file::getLastChangeCrc( id );
			}

			Dependency* pDependency = nullptr;
			for (uint i = 0u; i < dependencies.getCount(); ++i)
			{
				Dependency& dep = dependencies[ i ];

				if ( dep.type == type && dep.identifier == id )
				{
					pDependency = &dep;
					break;
				}
			}

			if ( pDependency == nullptr )
			{
				pDependency = &dependencies.add();
				pDependency->type		= type;
				pDependency->identifier	= id;
			}

			pDependency->valueText	= valueText;
			pDependency->valueInt	= valueInt;
		}

	private:

		List< TraceInfo >	traceInfos;
		List< OutputFile >	outputFiles;
		List< Dependency >	dependencies;
	};
}

#endif // TIKI_CONVERSIONPARAMETERS_HPP
