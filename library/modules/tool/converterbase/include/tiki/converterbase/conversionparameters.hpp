#pragma once
#ifndef TIKI_CONVERSIONPARAMETERS_HPP
#define TIKI_CONVERSIONPARAMETERS_HPP

#include "tiki/base/file.hpp"
#include "tiki/base/iopath.hpp"
#include "tiki/base/platform.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/toolbase/list.hpp"
#include "tiki/toolbase/map.hpp"

namespace tiki
{
	class ConversionArguments
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

		Map< string, string >			m_arguments;

		bool							getArgument( string& value, const string& key ) const; 

	};

	struct ConversionParameters
	{
		struct InputFile
		{
			string	fileName;
			crc32	typeCrc;
		};

		uint					assetId;
		bool					isBuildRequired;

		PlatformType			targetPlatform;
		GraphicsApi				targetApi;
		crc32					typeCrc;

		string					sourceFile;
		string					outputName;

		List< InputFile >		inputFiles;
		ConversionArguments		arguments;
	};

	class ConversionResult
	{
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

		struct Dependency
		{
			DependencyType	type;
			string			identifier;
			
			int				valueInt;
		};

		const List< TraceInfo >& getTraceInfos() const
		{
			return m_traceInfos;
		}

		const List< string >& getOutputFiles() const
		{
			return m_outputFiles;
		}

		const List< Dependency >& getDependencies() const
		{
			return m_dependencies;
		}

		void addTraceInfo( TraceLevel level, const string& message )
		{
			TraceInfo& traceInfo = m_traceInfos.add();
			traceInfo.level		= level;
			traceInfo.message	= message;
		}

		void addOutputFile( const string& fileName )
		{
			for (uint i = 0u; i < m_outputFiles.getCount(); ++i)
			{
				if ( m_outputFiles[ i ] == fileName )
				{
					return;
				}
			}

			m_outputFiles.add( fileName );
		}

		void addDependency( DependencyType type, const string& identifier, int valueInt )
		{
			string id = identifier;
			if ( type == DependencyType_File )
			{
				id			= path::getAbsolutePath( id );
				valueInt	= file::getLastChangeCrc( id.cStr() );
			}

			Dependency* pDependency = nullptr;
			for (uint i = 0u; i < m_dependencies.getCount(); ++i)
			{
				Dependency& dep = m_dependencies[ i ];

				if ( dep.type == type && dep.identifier == id )
				{
					pDependency = &dep;
					break;
				}
			}

			if ( pDependency == nullptr )
			{
				pDependency = &m_dependencies.add();
				pDependency->type		= type;
				pDependency->identifier	= id;
			}

			pDependency->valueInt	= valueInt;
		}

	private:

		List< TraceInfo >	m_traceInfos;
		List< string >		m_outputFiles;
		List< Dependency >	m_dependencies;

	};
}

#endif // TIKI_CONVERSIONPARAMETERS_HPP
