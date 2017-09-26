#pragma once
#ifndef TIKI_CONVERTERMANAGER_HPP
#define TIKI_CONVERTERMANAGER_HPP

#include "tiki/base/path.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/list.hpp"
#include "tiki/container/map.hpp"
#include "tiki/container/staticarray.hpp"
#include "tiki/converterbase/sqlite.hpp"
#include "tiki/io/filestream.hpp"
#include "tiki/tasksystem/tasksystem.hpp"
#include "tiki/threading/mutex.hpp"

#include "tiki/animationconverter/animationconverter.hpp"
#include "tiki/fontconverter/fontconverter.hpp"
#include "tiki/genericdataconverter/genericdataconverter.hpp"
#include "tiki/modelconverter/modelconverter.hpp"
#include "tiki/shaderconverter/shaderconverter.hpp"
#include "tiki/textureconverter/textureconverter.hpp"

namespace tiki
{
	class ConversionResult;
	class ConverterBase;
	class XmlReader;
	struct ConversionParameters;

	struct ConverterManagerParameter
	{
		ConverterManagerParameter()
		{
			pChangedFilesList	= nullptr;

			forceRebuild		= false;
		}

		string			sourcePath;
		string			outputPath;
		string			packageName;

		List< string >*	pChangedFilesList;

		bool			forceRebuild;
	};

	class ConverterManager
	{
		TIKI_NONCOPYABLE_CLASS( ConverterManager );

		friend void globalTraceCallback( const char* message, TraceLevel level, UserData userData );
		friend class ConverterBase;

	public:

		~ConverterManager();

		bool					create( const ConverterManagerParameter& parameters );
		void					dispose();

		// conversion
		void					queueFile( const string& fileName );
		bool					startConversion( Mutex* pConversionMutex = nullptr );

		// misc
		//const string&			getSourcePath() const { return m_sourcePath; }
		//const string&			getOutputPath() const { return m_outputPath; }
		bool					isNewDatabase() const { return m_isNewDatabase; }

	private: // friend

		// converter
		void					registerConverter( const ConverterBase* pConverter );
		void					unregisterConverter( const ConverterBase* pConverter );

		// task system
		TaskId					queueTask( TaskFunc pFunc, void* pData, TaskId dependingTaskId = InvalidTaskId );
		void					waitForTask( TaskId taskId );

	private:

		typedef List< const ConverterBase* > ConverterList;

		struct FileDescription
		{
			Path		filePath;
			crc32		fileType;
		};
		typedef List< FileDescription > FileList;

		struct TemplateDescription
		{
			string					fullFileName;
			string					name;

			Map< string, string >	arguments;
		};
		typedef Map< string, TemplateDescription > TemplateMap;

		struct ConversionTask
		{
			ConverterManager*		pManager;
			const ConverterBase*	pConverter;

			ConversionParameters	parameters;
			ConversionResult		result;

			TaskId					taskId;
		};

		typedef Map< uint64, ConversionResult* > ThreadResultMap;

		string						m_sourcePath;
		string						m_outputPath;

		SqliteDatabase				m_dataBase;
		bool						m_rebuildForced;
		bool						m_isNewDatabase;

		AnimationConverter			m_animationConverter;
		FontConverter				m_fontConverter;
		GenericDataConverter		m_genericDataConverter;
		ModelConverter				m_modelConverter;
		ShaderConverter				m_shaderConverter;
		TextureConverter			m_textureConverter;

		Mutex						m_loggingMutex;
		FileStream					m_loggingStream;
		ThreadResultMap				m_loggingThreadResults;

		TemplateMap					m_templates;
		ConverterList				m_converters;

		List< string >				m_files;
		List< string >*				m_pChangedFilesList;

		TaskSystem					m_taskSystem;
		List< ConversionTask >		m_tasks;

		void						traceCallback( const char* message, TraceLevel level );

		void						addPackage( const string& packageName );
		void						addTemplate( const string& fileName );

		bool						prepareTasks();
		bool						generateTaskFromFiles( const List< FileDescription >& filesToBuild );
		bool						readDataFromXasset( ConversionTask& task, const FileDescription& fileDesc );
		bool						writeConvertInputs( List< ConversionTask >& tasks );
		bool						checkDependencies( List< ConversionTask >& tasks );

		bool						finalizeTasks();

		static void					taskConvertFile( const TaskContext& context );
		void						taskRegisterResult( uint64 threadId, ConversionResult& result );
		void						taskUnregisterResult( uint64 threadId );

	};
}

#endif // TIKI_CONVERTERMANAGER_HPP
