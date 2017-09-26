#pragma once
#ifndef TIKI_CONVERSION_RESULT_HPP_INCLUDED
#define TIKI_CONVERSION_RESULT_HPP_INCLUDED

#include "tiki/base/platform.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/list.hpp"
#include "tiki/container/map.hpp"
#include "tiki/io/file.hpp"
#include "tiki/io/path.hpp"

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
		const List< string >&		getOutputFiles() const;
		const List< Dependency >&	getDependencies() const;

		void						addTraceInfo( TraceLevel level, const string& message );
		void						addOutputFile( const string& fileName );
		void						addDependency( DependencyType type, string identifier, string value );

	private:

		List< TraceInfo >	m_traceInfos;
		List< string >		m_outputFiles;
		List< Dependency >	m_dependencies;
	};
}

#endif // TIKI_CONVERSION_RESULT_HPP_INCLUDED
