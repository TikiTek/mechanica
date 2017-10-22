#pragma once
#ifndef TIKI_CONVERTERMANAGER_HPP
#define TIKI_CONVERTERMANAGER_HPP

#include "tiki/base/path.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/list.hpp"
#include "tiki/container/map.hpp"
#include "tiki/container/staticarray.hpp"
#include "tiki/converterbase/conversion_asset.hpp"
#include "tiki/converterbase/conversion_result.hpp"
#include "tiki/converterbase/sqlite.hpp"
#include "tiki/io/filestream.hpp"
#include "tiki/tasksystem/tasksystem.hpp"
#include "tiki/threading/mutex.hpp"

#include "tiki/animationconverter/animation_converter.hpp"
#include "tiki/fontconverter/font_converter.hpp"
#include "tiki/genericdataconverter/generic_data_converter.hpp"
#include "tiki/modelconverter/model_converter.hpp"
#include "tiki/shaderconverter/shader_converter.hpp"
#include "tiki/textureconverter/texture_converter.hpp"

namespace tiki
{
	class ConversionResult;
	class ConverterBase;
	class XmlReader;

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

		ConverterManager();
		~ConverterManager();

		bool					create( const ConverterManagerParameter& parameters );
		void					dispose();

		// conversion
		void					queueFile( const Path& filePath );
		bool					startConversion( Mutex* pConversionMutex = nullptr );

		// misc
		//const string&			getSourcePath() const { return m_sourcePath; }
		const string&			getOutputPath() const { return m_outputPath; }
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

		struct TemplateDescription
		{
			Path					filePath;
			string					name;

			ConversionParameters	parameters;
		};
		typedef Map< string, TemplateDescription > TemplateMap;

		struct ConversionTask
		{
			ConverterManager*		pManager;
			const ConverterBase*	pConverter;

			ConversionAsset			asset;
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

		List< Path >				m_files;
		List< string >*				m_pChangedFilesList;

		TaskSystem					m_taskSystem;
		List< ConversionTask >		m_tasks;

		void						traceCallback( const char* message, TraceLevel level );

		void						addPackage( const string& packageName );
		void						addTemplate( const Path& filePath );

		bool						prepareTasks();
		bool						fillAssetFromFilePath( ConversionAsset& asset, const Path& filePath );
		bool						generateTaskFromFiles( const List< ConversionAsset >& assetsToBuild );
		bool						writeConvertInputs( List< ConversionTask >& tasks );
		bool						checkDependencies( List< ConversionTask >& tasks );

		bool						finalizeTasks();

		static void					taskConvertFile( const TaskContext& context );
		void						taskRegisterResult( uint64 threadId, ConversionResult& result );
		void						taskUnregisterResult( uint64 threadId );

	};
}

#endif // TIKI_CONVERTERMANAGER_HPP
