#pragma once

#include "tiki/base/path.hpp"
#include "tiki/container/list.hpp"

namespace tiki
{
	class ConversionResult
	{
	public:

		enum DependencyType
		{
			DependencyType_InputFile,
			DependencyType_OutputFile,
			DependencyType_Parameter,
			DependencyType_Type,
			DependencyType_Converter,

			DependencyType_Count
		};

		struct TraceInfo
		{
			TraceLevel				level;
			DynamicString			message;
		};

		struct Dependency
		{
			DependencyType			type;
			DynamicString			identifier;
			sint64					intValue;
		};

		const List< TraceInfo >&	getTraceInfos() const;
		const List< Path >&			getOutputFiles() const;
		const List< Dependency >&	getDependencies() const;

		void						addInputFile( const Path& filePath );
		void						addOutputFile( const Path& filePath );
		void						addDependency( DependencyType type, const DynamicString& identifier, sint64 intValue );
		void						addTraceInfo( TraceLevel level, const DynamicString& message );

	private:

		List< TraceInfo >			m_traceInfos;
		List< Path >				m_outputFiles;
		List< Dependency >			m_dependencies;
	};
}
