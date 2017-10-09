#pragma once
#ifndef TIKI_CONVERSION_RESULT_HPP_INCLUDED
#define TIKI_CONVERSION_RESULT_HPP_INCLUDED

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
			TraceLevel	level;
			string		message;
		};

		struct Dependency
		{
			DependencyType	type;
			string			identifier;
			string			value;
		};

		const List< TraceInfo >&	getTraceInfos() const;
		const List< Path >&			getOutputFiles() const;
		const List< Dependency >&	getDependencies() const;

		void						addInputFile( const Path& filePath );
		void						addOutputFile( const Path& filePath );
		void						addDependency( DependencyType type, string identifier, string value );
		void						addTraceInfo( TraceLevel level, const string& message );

	private:

		List< TraceInfo >	m_traceInfos;
		List< Path >		m_outputFiles;
		List< Dependency >	m_dependencies;
	};
}

#endif // TIKI_CONVERSION_RESULT_HPP_INCLUDED
