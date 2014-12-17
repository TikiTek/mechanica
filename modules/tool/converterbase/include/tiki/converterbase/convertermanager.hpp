#pragma once
#ifndef TIKI_CONVERTERMANAGER_HPP
#define TIKI_CONVERTERMANAGER_HPP

#include "tiki/base/staticarray.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/converterbase/conversionparameters.hpp"
#include "tiki/converterbase/converterresourcemap.hpp"
#include "tiki/io/filestream.hpp"
#include "tiki/threading/mutex.hpp"
#include "tiki/toolbase/list.hpp"
#include "tiki/toolbase/sqlite.hpp"
#include "tiki/tasksystem/tasksystem.hpp"

#include <map>

struct _XmlElement;

namespace tiki
{
	class ConversionResult;
	class ConverterBase;
	class TikiXml;
	struct ConversionParameters;

	struct ConverterManagerParameter
	{
		ConverterManagerParameter()
		{
			forceRebuild	= false;
		}

		string	outputPath;

		bool	forceRebuild;
	};

	class ConverterManager
	{
		friend void globalTraceCallback( cstring message, TraceLevel level );

	public:

		~ConverterManager();

		void					create( const ConverterManagerParameter& parameters );
		void					dispose();

		// conversion
		void					addTemplate( const string& fileName );
		void					queueFile( const string& fileName );
		bool					startConversion( List< string >* pOutputFiles = nullptr, Mutex* pConversionMutex = nullptr );

		// converter
		void					registerConverter( const ConverterBase* pConverter );
		void					unregisterConverter( const ConverterBase* pConverter );

		// task system
		TaskId					queueTask( TaskFunc pFunc, void* pData, TaskId dependingTaskId = InvalidTaskId );
		void					waitForTask( TaskId taskId );

		// misc
		const string&			getOutputPath() const { return m_outputPath; }
		bool					isNewDatabase() const { return m_isNewDatabase; }

	private:

		typedef List< const ConverterBase* > ConverterList;

		struct FileDescription
		{
			string		fullFileName;
			crc32		fileType;
		};
		typedef List< FileDescription > FileList;

		struct TemplateDescription
		{
			string						fullFileName;
			string						name;

			std::map< string, string >	arguments;
		};
		typedef std::map< string, TemplateDescription > TemplateMap;

		struct ConversionTask
		{
			ConverterManager*		pManager;
			const ConverterBase*	pConverter;

			ConversionParameters	parameters;
			ConversionResult		result;

			TaskId					taskId;
		};

		typedef std::map< uint64, ConversionResult* > ThreadResultMap;

		string						m_outputPath;

		SqliteDatabase				m_dataBase;
		bool						m_rebuildForced;
		bool						m_isNewDatabase;

		mutable Mutex				m_loggingMutex;
		mutable FileStream			m_loggingStream;
		mutable ThreadResultMap		m_loggingThreadResults;

		TemplateMap					m_templates;
		ConverterList				m_converters;
		ConverterResourceMap		m_resourceMap;

		List< string >				m_files;

		TaskSystem					m_taskSystem;
		List< ConversionTask >		m_tasks;

		void						traceCallback( cstring message, TraceLevel level ) const;
		void						parseParams( const TikiXml& xmlFile, const _XmlElement* pRoot, std::map< string, string >& arguments ) const;

		bool						prepareTasks();
		bool						generateTaskFromFiles( const List< FileDescription >& filesToBuild );
		bool						readDataFromXasset( ConversionTask& task, const FileDescription& fileDesc );
		bool						writeConvertInputs( List< ConversionTask >& tasks );
		bool						checkDependencies( List< ConversionTask >& tasks );

		bool						finalizeTasks( List< string >& outputFiles );

		static void					taskConvertFile( const TaskContext& context );
		void						taskRegisterResult( uint64 threadId, ConversionResult& result );
		void						taskUnregisterResult( uint64 threadId );

	};
}

#endif // TIKI_CONVERTERMANAGER_HPP
