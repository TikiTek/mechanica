#include "tiki/converter_base/conversion_result.hpp"

#include "tiki/io/path.hpp"
#include "tiki/io/file.hpp"

namespace tiki
{
	const List< ConversionResult::TraceInfo >& ConversionResult::getTraceInfos() const
	{
		return m_traceInfos;
	}

	const List< Path >& ConversionResult::getOutputFiles() const
	{
		return m_outputFiles;
	}

	const List< ConversionResult::Dependency >& ConversionResult::getDependencies() const
	{
		return m_dependencies;
	}

	void ConversionResult::addInputFile( const Path& filePath )
	{
		addDependency( DependencyType_InputFile, filePath.getCompletePath(), 0 );
	}

	void ConversionResult::addOutputFile( const Path& filePath )
	{
		for( uint i = 0u; i < m_outputFiles.getCount(); ++i )
		{
			if( isStringEquals( m_outputFiles[ i ].getCompletePath(), filePath.getCompletePath() ) )
			{
				return;
			}
		}

		m_outputFiles.add( filePath );
		addDependency( DependencyType_OutputFile, filePath.getCompletePath(), 0 );
	}

	void ConversionResult::addDependency( DependencyType type, const DynamicString& identifier, sint64 intValue )
	{
		Dependency* pDependency = nullptr;
		for( uint i = 0u; i < m_dependencies.getCount(); ++i )
		{
			Dependency& dep = m_dependencies[ i ];

			if( dep.type == type && dep.identifier == identifier )
			{
				pDependency = &dep;
				break;
			}
		}

		if( pDependency == nullptr )
		{
			pDependency = &m_dependencies.add();
			pDependency->type		= type;
			pDependency->identifier	= identifier;
		}

		if( type == DependencyType_InputFile )
		{
			pDependency->intValue = file::getLastChangeCrc( identifier.cStr() );
		}
		else
		{
			pDependency->intValue = intValue;
		}
	}

	void ConversionResult::addTraceInfo( TraceLevel level, const DynamicString& message )
	{
		TraceInfo& traceInfo = m_traceInfos.add();
		traceInfo.level		= level;
		traceInfo.message	= message;
	}
}
