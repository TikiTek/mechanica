#pragma once
#ifndef TIKI_ASSET_CONVERTER_HPP
#define TIKI_ASSET_CONVERTER_HPP

#include "tiki/asset_converter_interface/asset_converter_interface.hpp"

#include "tiki/container/list.hpp"
#include "tiki/converter_base/conversion_parameters.hpp"
#include "tiki/converter_base/conversion_result.hpp"
#include "tiki/converter_base/conversion_types.hpp"
#include "tiki/io/file_stream.hpp"
#include "tiki/io/file_watcher.hpp"
#include "tiki/task_system/task_system.hpp"
#include "tiki/threading/thread.hpp"
#include "tiki/tool_sql/sqlite_database.hpp"

#include "tiki/animation_converter/animation_converter.hpp"
#include "tiki/font_converter/font_converter.hpp"
#include "tiki/generic_data_converter/generic_data_converter.hpp"
#include "tiki/model_converter/model_converter.hpp"
#include "tiki/shader_converter/shader_converter.hpp"
#include "tiki/texture_converter/texture_converter.hpp"

namespace tiki
{
	class ConverterBase;

	class AssetConverter : public AssetConverterInterface
	{
		TIKI_NONCOPYABLE_CLASS( AssetConverter );
		friend void globalTraceCallback( const char* message, TraceLevel level, UserData userData );

	public:

						AssetConverter();
		virtual			~AssetConverter();

		virtual bool	create( const AssetConverterParamter& parameters );
		virtual void	dispose();

		virtual bool	convertAll() TIKI_OVERRIDE_FINAL;
		void			queueFile( const Path& filePath );
		bool			startConversion( Mutex* pConversionMutex = nullptr );

		virtual void	startWatch() TIKI_OVERRIDE_FINAL;
		virtual void	stopWatch() TIKI_OVERRIDE_FINAL;

		virtual bool	getChangedFiles( Array< string >& changedFiles ) TIKI_OVERRIDE_FINAL;

	private:

		typedef List< const ConverterBase* > ConverterList;

		struct TemplateDescription
		{
			Path					filePath;
			string					name;
			crc32					typeCrc;

			ConversionParameters	parameters;
		};
		typedef Map< string, TemplateDescription > TemplateMap;

		struct ConversionTask
		{
			AssetConverter*			pAssetConverter;
			const ConverterBase*	pConverter;

			ConversionAsset			asset;
			ConversionResult		result;

			TaskId					taskId;
		};

		typedef Map< uint64, ConversionResult* > ThreadResultMap;

		const Project*				m_pProject;
		bool						m_ownsProject;
		ConversionContext			m_context;

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

		ConverterList				m_converters;
		TemplateMap					m_templates;
		Map< string, crc32 >		m_extensions;

		List< Path >				m_files;
		Mutex						m_changedFilesMutex;
		List< string >				m_changedFiles;

		Mutex						m_watcherMutex;
		Thread						m_watcherThread;
		FileWatcher					m_watcher;

		TaskSystem					m_taskSystem;
		List< ConversionTask >		m_tasks;

		const ConversionContext&	getContext() const { return m_context; }

		void						registerConverter( const ConverterBase* pConverter );
		void						unregisterConverter( const ConverterBase* pConverter );

		void						addPackage( const Package& package );
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

		void						watcherThreadEntryPoint( const Thread& thread );
		static int					watcherThreadStaticEntryPoint( const Thread& thread );

		void						traceCallback( const char* message, TraceLevel level );
	};
}

#endif // TIKI_ASSET_CONVERTER_HPP
