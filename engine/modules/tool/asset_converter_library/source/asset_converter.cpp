#include "asset_converter.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/base/string_tools.hpp"
#include "tiki/io/directory.hpp"
#include "tiki/io/file.hpp"
#include "tiki/task_system/task_context.hpp"
#include "tiki/tool_base/directory_tool.hpp"
#include "tiki/tool_project/package.hpp"
#include "tiki/tool_project/project.hpp"
#include "tiki/tool_sql/sqlite_query.hpp"
#include "tiki/tool_xml/xml_attribute.hpp"
#include "tiki/tool_xml/xml_document.hpp"
#include "tiki/tool_xml/xml_element.hpp"

namespace tiki
{
	static void globalTraceCallback( const char* message, TraceLevel level, UserData userData )
	{
		AssetConverter* pAssetConverter = (AssetConverter*)userData.pContext;
		pAssetConverter->traceCallback( message, level );
	}

	static DynamicString escapeString( const DynamicString& text )
	{
		return text.replace( "'", "''" ).replace( "\"", "\"\"" );
	}

	TIKI_ASSET_CONVERTER_METHOD AssetConverterInterface* createAssetConverter( const AssetConverterParamter& parameters )
	{
		AssetConverter* pAssetConverter = TIKI_NEW( AssetConverter );
		if( !pAssetConverter->create( parameters ) )
		{
			TIKI_DELETE( pAssetConverter );
			pAssetConverter = nullptr;
		}

		return pAssetConverter;
	}

	TIKI_ASSET_CONVERTER_METHOD void disposeAssetConverter( AssetConverterInterface* pAssetConverterInterface )
	{
		if( pAssetConverterInterface == nullptr )
		{
			return;
		}

		AssetConverter* pAssetConverter = ( AssetConverter* )pAssetConverterInterface;
		pAssetConverter->dispose();
		TIKI_DELETE( pAssetConverter );
	}

	AssetConverter::AssetConverter()
	{
	}

	AssetConverter::~AssetConverter()
	{
		TIKI_ASSERT( m_converters.isEmpty() );
	}

	bool AssetConverter::create( const AssetConverterParamter& parameters )
	{
		m_pProject		= parameters.pProject;
		m_ownsProject	= false;
		m_rebuildForced = parameters.forceRebuild;
		m_queuedFilesMutex.create();
		m_changedFilesMutex.create();

		if( m_pProject == nullptr )
		{
			m_pProject = TIKI_NEW( Project );
			m_ownsProject = true;
		}

		m_context.pProject = m_pProject;

		TaskSystemParameters taskParameters;
		taskParameters.maxTaskCount = 1024u;
		//taskParameters.threadCount	= 0u;
		m_taskSystem.create( taskParameters );

		if( !directory::exists( m_pProject->getAssetBuildPath().getCompletePath() ) )
		{
			directory::create( m_pProject->getAssetBuildPath().getCompletePath() );
		}

		Path databasePath = m_pProject->getAssetBuildPath();
		databasePath.push( "build.sqlite" );

		m_isNewDatabase = !file::exists( databasePath.getCompletePath() );
		if( !m_dataBase.create( databasePath.getCompletePath() ) )
		{
			TIKI_TRACE_ERROR( "[converter] database intialization failed.\n" );
			return false;
		}

		if( m_isNewDatabase )
		{
			TIKI_TRACE_INFO( "[converter] Database not found. Create new Database.\n" );

			const char* pCreateTableSql[] =
			{
				"CREATE TABLE builds (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, buildtime DATETIME, has_error BOOL);",
				"CREATE TABLE assets (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, filename TEXT NOT NULL, path TEXT NOT NULL, type TEXT NOT NULL, has_error BOOL);",
				"CREATE TABLE dependencies (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, asset_id INTEGER NOT NULL, type INTEGER, identifier TEXT, value_int BIGINT);",
				"CREATE TABLE input_files (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, asset_id INTEGER NOT NULL, filename TEXT NOT NULL, type INTEGER NOT NULL);",
				"CREATE TABLE output_files (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, asset_id INTEGER NOT NULL, filename TEXT NOT NULL);",
				"CREATE TABLE traces (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, asset_id INTEGER NOT NULL, level INTEGER, message TEXT);"
			};

			for( uintreg i = 0u; i < TIKI_COUNT( pCreateTableSql ); ++i )
			{
				if( !m_dataBase.executeCommand( pCreateTableSql[ i ] ) )
				{
					TIKI_TRACE_ERROR( "[converter] Could not create Table. Error: %s\n", m_dataBase.getLastError() );
					m_dataBase.dispose();
					return false;
				}
			}
		}
		else
		{
			SqliteQuery query;
			if( query.create( m_dataBase, "SELECT * FROM builds WHERE has_error = 0 LIMIT 1;" ) )
			{
				m_isNewDatabase = !query.nextRow();
			}
			else
			{
				TIKI_TRACE_ERROR( "[converter] Unable to read from builds table. Error: %s\n", m_dataBase.getLastError() );
				m_dataBase.dispose();
				return false;
			}
			query.dispose();
		}

		m_loggingMutex.create();
		m_loggingStream.create( "converter.log", DataAccessMode_WriteAppend );

		debug::setTraceCallback( globalTraceCallback, UserData( this ) );

		ConverterBase* apConverters[] =
		{
			&m_animationConverter,
			&m_fontConverter,
			&m_genericDataConverter,
			&m_modelConverter,
			&m_shaderConverter,
			&m_textureConverter
		};

		ConversionContext context;
		context.pProject = m_pProject;

		for( uintreg i = 0u; i < TIKI_COUNT( apConverters ); ++i )
		{
			ConverterBase* pConverter = apConverters[ i ];

			pConverter->create( m_pProject->getAssetBuildPath(), &m_taskSystem, context );

			List< DynamicString > extensions;
			pConverter->getInputExtensions( extensions );
			for( const DynamicString& extenstion : extensions )
			{
				m_extensions.set( extenstion, pConverter->getOutputType() );
			}

			registerConverter( pConverter );
		}

		for( const Package& package : m_pProject->getPackages() )
		{
			addPackage( package );
		}

		if( (m_isNewDatabase && parameters.rebuildOnMissingDatabase) || parameters.waitForConversion )
		{
			queueAll();
		}

		TIKI_TRACE_INFO( "[converter] started\n" );
		return true;
	}

	void AssetConverter::dispose()
	{
		TIKI_TRACE_INFO( "[converter] shutdown\n" );

		ConverterBase* apConverters[] =
		{
			&m_animationConverter,
			&m_fontConverter,
			&m_genericDataConverter,
			&m_modelConverter,
			&m_shaderConverter,
			&m_textureConverter
		};

		for( uintreg i = 0u; i < TIKI_COUNT( apConverters ); ++i )
		{
			ConverterBase* pConverter = apConverters[ i ];

			unregisterConverter( pConverter );
			pConverter->dispose();
		}

		debug::setTraceCallback( nullptr );

		m_taskSystem.dispose();

		m_loggingStream.dispose();
		m_loggingMutex.dispose();
		m_loggingThreadResults.dispose();

		m_dataBase.dispose();

		if( m_ownsProject )
		{
			TIKI_DELETE( m_pProject );
			m_pProject = nullptr;
		}

		m_changedFilesMutex.dispose();
		m_queuedFilesMutex.dispose();
	}

	void AssetConverter::queueAll()
	{
		List< Path > assetFiles;
		for( const KeyValuePair< DynamicString, DynamicString >& extension : m_extensions )
		{
			directory::findFiles( assetFiles, m_pProject->getContentPath(), extension.key );
		}

		MutexStackLock lock( m_queuedFilesMutex );
		for( const Path& file : assetFiles )
		{
			if( m_queuedFiles.contains( file ) )
			{
				continue;
			}

			m_queuedFiles.add( file );
		}
		TIKI_TRACE_INFO( "[converter] Scan complete: %u elements queued.\n", assetFiles.getCount() );
	}

	void AssetConverter::startWatch()
	{
		m_watcher.create( m_pProject->getContentPath().getCompletePath(), 32u );
		m_watcherThread.create( watcherThreadStaticEntryPoint, this, 8192u, "AssetConverter" );
	}

	void AssetConverter::stopWatch()
	{
		if ( m_watcherThread.isCreated() )
		{
			m_watcherThread.requestExit();
			m_watcherThread.waitForExit();
			m_watcherThread.dispose();
		}

		m_watcher.dispose();
	}

	bool AssetConverter::isConvertionRunning()
	{
		return m_threadWork.getRelaxed();
	}

	bool AssetConverter::popFinishConversion()
	{
		return m_threadFinish.andRelaxed( false );
	}

	bool AssetConverter::wasLastBuildSuccessful()
	{
		return m_threadSuccessful.andRelaxed( false );
	}

	bool AssetConverter::getChangedFiles( Array< DynamicString >& changedFiles )
	{
		MutexStackLock lock( m_changedFilesMutex );

		if( !m_changedFiles.isEmpty() )
		{
			changedFiles.create( m_changedFiles.getBegin(), m_changedFiles.getCount() );
			m_changedFiles.clear();

			return true;
		}

		return false;
	}

	bool AssetConverter::startConversion()
	{
		List< ConversionTask > tasks;
		if( !prepareTasks( tasks ) )
		{
			return false;
		}

		for( uintreg i = 0u; i < tasks.getCount(); ++i )
		{
			ConversionTask& task = tasks[ i ];
			task.taskId = m_taskSystem.queueTask( taskConvertFile, &task );
		}

		m_taskSystem.waitForAllTasks();

		if( !finalizeTasks( tasks ) )
		{
			return false;
		}

		return true;
	}

	void AssetConverter::registerConverter( const ConverterBase* pConverter )
	{
		m_converters.add( pConverter );
	}

	void AssetConverter::unregisterConverter( const ConverterBase* pConverter )
	{
		m_converters.removeSortedByValue( pConverter );
	}

	void AssetConverter::traceCallback( const char* message, TraceLevel level )
	{
		DynamicString line = message;
		if( !line.endsWith( '\n' ) )
		{
			line += "\n";
		}

		const uint64 currentThreadId = Thread::getCurrentThreadId();

		m_loggingMutex.lock();

		m_loggingStream.write( line.cStr(), line.getLength() );

		ConversionResult* pResult = nullptr;
		if( m_loggingThreadResults.findValue( &pResult, currentThreadId ) && pResult != nullptr )
		{
			pResult->addTraceInfo( level, message );
		}

		m_loggingMutex.unlock();
	}

	void AssetConverter::addPackage( const Package& package )
	{
		List< Path > templates;
		package.findAssetTemplateFiles( templates );
		for( const Path& templatePath : templates )
		{
			addTemplate( templatePath );
		}
	}

	void AssetConverter::addTemplate( const Path& filePath )
	{
		XmlDocument document;
		if( !document.loadFromFile( filePath.getCompletePath() ) )
		{
			return;
		}

		const XmlElement* pRoot = document.findFirstChild( "template" );
		if( pRoot == nullptr )
		{
			TIKI_TRACE_ERROR( "[converter] can't find 'template' root node in '%s'.\n", filePath.getCompletePath() );
			return;
		}

		const XmlAttribute* pBaseAtt = pRoot->findAttribute( "base" );
		if( pBaseAtt == nullptr )
		{
			TIKI_TRACE_ERROR( "[converter] 'base' attribute not found. can't add template: %s\n", filePath.getCompletePath() );
			return;
		}

		TemplateDescription desc;
		desc.name		= filePath.getBasename();
		desc.filePath	= filePath;
		desc.type		= pBaseAtt->getValue();

		const XmlElement* pParameterNode = pRoot->findFirstChild( "parameter" );
		while( pParameterNode )
		{
			const XmlAttribute* pAttKey		= pParameterNode->findAttribute( "key" );
			const XmlAttribute* pAttValue	= pParameterNode->findAttribute( "value" );

			if( pAttKey == nullptr )
			{
				TIKI_TRACE_WARNING(
					"param failed: %s%s\n",
					(pAttKey == nullptr ? "no key-attribute " : DynamicString( "key: " ) + pAttKey->getValue()).cStr(),
					(pAttValue == nullptr ? "no value-attribute " : DynamicString( "value: " ) + pAttValue->getValue()).cStr()
				);
			}
			else if( pAttValue == nullptr )
			{
				desc.parameters.addParameter( pAttKey->getValue(), pParameterNode->getValue() );
			}
			else
			{
				desc.parameters.addParameter( pAttKey->getValue(), pAttValue->getValue() );
			}

			pParameterNode = pParameterNode->findNextSibling( "parameter" );
		}

		m_templates.set( desc.name, desc );
	}

	bool AssetConverter::prepareTasks( List< ConversionTask >& tasks )
	{
		List< ConversionAsset > assetsToBuild;
		List< Path > filesFromDependencies;

		{
			MutexStackLock lock( m_queuedFilesMutex );

			for( uintreg i = 0u; i < m_queuedFiles.getCount(); ++i )
			{
				const Path& file = m_queuedFiles[ i ];

				ConversionAsset& asset = assetsToBuild.pushBack();
				if( !fillAssetFromFilePath( asset, file ) )
				{
					assetsToBuild.popBack();
					filesFromDependencies.add( file );
				}
			}
			m_queuedFiles.clear();
		}

		if( filesFromDependencies.getCount() > 0u )
		{
			DynamicString whereFileName;
			for( uintreg i = 0u; i < filesFromDependencies.getCount(); ++i )
			{
				const Path& filePath = filesFromDependencies[ i ];

				if( i != 0u )
				{
					whereFileName += " OR ";
				}

				whereFileName += "dep.identifier = '";
				whereFileName += filePath.getCompletePath();
				whereFileName += "'";
			}

			{
				const DynamicString sql = formatDynamicString( "SELECT asset.* FROM dependencies as dep, assets as asset WHERE dep.type = '%u' AND asset.id = dep.asset_id AND (%s)", ConversionResult::DependencyType_InputFile, whereFileName.cStr() );

				SqliteQuery query;
				if( !query.create( m_dataBase, sql.cStr() ) )
				{
					TIKI_TRACE_ERROR( "[converter] SQL command failed. Error: %s\n", query.getLastError() );
					return false;
				}

				while( query.nextRow() )
				{
					ConversionAsset asset;
					asset.inputFilePath.setCombinedPath( query.getTextField( "path" ), query.getTextField( "filename" ) );
					asset.assetName	= asset.inputFilePath.getBasename();
					asset.type		= query.getTextField( "type" );
					asset.assetId	= TIKI_SIZE_T_MAX;

					bool found = false;
					for( uintreg i = 0u; i < assetsToBuild.getCount(); ++i )
					{
						if( assetsToBuild[ i ].assetName == asset.assetName )
						{
							found = true;
							break;
						}
					}

					if( !found )
					{
						assetsToBuild.add( asset );
					}
				}
			}
		}

		tasks.clear();

		return generateTaskFromFiles( tasks, assetsToBuild );
	}

	bool AssetConverter::fillAssetFromFilePath( ConversionAsset& asset, const Path& filePath )
	{
		Path assetPath;
		assetPath.setCompletePath( filePath.getBasename() );

		asset.inputFilePath = filePath;
		asset.assetName		= assetPath.getBasename();
		asset.assetId		= TIKI_SIZE_T_MAX;

		if( !isStringEmpty( assetPath.getExtension() ) )
		{
			const DynamicString templateName = assetPath.getExtension() + 1u;
			if( m_templates.hasKey( templateName ) )
			{
				const TemplateDescription& templateDescription = m_templates[ templateName ];

				asset.type = templateDescription.type;
				asset.parameters.copyFrom( templateDescription.parameters );
			}
			else
			{
				asset.type = templateName;
			}

			return true;
		}
		else if( isStringEmpty( filePath.getExtension() ) )
		{
			TIKI_TRACE_ERROR( "[converter] '%s' has no type or template in its name.\n", filePath.getCompletePath() );
			return false;
		}

		if( !m_extensions.findValue( &asset.type, filePath.getExtension() ) )
		{
			TIKI_TRACE_ERROR( "[converter] '%s' extension has no type.\n", filePath.getCompletePath() );
		}

		return true;
	}

	bool AssetConverter::generateTaskFromFiles( List< ConversionTask >& tasks, const List< ConversionAsset >& assetsToBuild )
	{
		List< ConversionTask > tempTasks;

		bool result = true;
		for( uintreg assetIndex = 0u; assetIndex < assetsToBuild.getCount(); ++assetIndex )
		{
			const ConversionAsset& asset = assetsToBuild[ assetIndex ];
			const crc32 typeCrc = crcString( asset.type );

			if( !file::exists( asset.inputFilePath.getCompletePath() ) )
			{
				TIKI_TRACE_ERROR( "[converter] File to convert not found: %s.\n", asset.inputFilePath.getCompletePath() );
				result = false;
				continue;
			}

			ConversionTask task;
			task.pConverter = nullptr;
			for( const ConverterBase* pConverter : m_converters )
			{
				if( pConverter->canConvertType( typeCrc ) )
				{
					task.pConverter = pConverter;
					break;
				}
			}

			if( task.pConverter == nullptr )
			{
				TIKI_TRACE_ERROR( "[converter] No Converter found for file: '%s'.\n", asset.inputFilePath.getCompletePath() );
				//result = false;
				continue;
			}

			task.asset					= asset;
			task.asset.isAlradyBuilt	= false;
			task.pAssetConverter		= this;

			task.result.addDependency( ConversionResult::DependencyType_Converter, "", task.pConverter->getConverterRevision( typeCrc ) );
			task.result.addInputFile( task.asset.inputFilePath );

			tempTasks.add( task );
		}

		if( !writeConvertInputs( tempTasks ) )
		{
			return false;
		}

		if( !checkDependencies( tempTasks ) )
		{
			return false;
		}

		for( uintreg i = 0u; i < tempTasks.getCount(); ++i )
		{
			ConversionTask& task = tempTasks[ i ];

			if( !task.asset.isAlradyBuilt )
			{
				tasks.add( task );
			}
		}

		return result;
	}

	bool AssetConverter::writeConvertInputs( List< ConversionTask >& tasks )
	{
		if( tasks.isEmpty() )
		{
			return true;
		}

		bool result = true;

		DynamicString whereFileName;
		Map< DynamicString, ConversionTask* > tasksByFileName;
		for( uintreg i = 0u; i < tasks.getCount(); ++i )
		{
			ConversionTask& task = tasks[ i ];
			const DynamicString fileName = task.asset.inputFilePath.getFilenameWithExtension();

			if( i != 0u )
			{
				whereFileName += " OR ";
			}

			whereFileName += "filename = '" + fileName + "'";

			tasksByFileName.set( fileName, &task );
		}

		// read asset ids
		DynamicString whereAssetId;
		{
			const DynamicString sql = "SELECT id, filename, has_error FROM assets WHERE " + whereFileName;

			SqliteQuery query;
			if( !query.create( m_dataBase, sql.cStr() ) )
			{
				TIKI_TRACE_ERROR( "[converter] SQL command failed. Error: %s\n", m_dataBase.getLastError() );
				return false;
			}

			while( query.nextRow() )
			{
				const DynamicString fileName = query.getTextField( "filename" );

				ConversionTask* pTask = nullptr;
				if( tasksByFileName.findValue( &pTask, fileName ) && pTask != nullptr )
				{
					pTask->asset.assetId		= query.getIntegerField( "id" );
					pTask->asset.isAlradyBuilt	= (query.getIntegerField( "has_error" ) == 0u);

					if( !whereAssetId.isEmpty() )
					{
						whereAssetId += " OR ";
					}
					whereAssetId += formatDynamicString( "asset_id = '%u'", pTask->asset.assetId );
				}
				else
				{
					TIKI_TRACE_ERROR( "[converter] Could not find task for '%s'.\n", fileName.cStr() );
				}
			}

			for( uintreg i = 0u; i < tasks.getCount(); ++i )
			{
				ConversionTask& task = tasks[ i ];
				if( task.asset.assetId != TIKI_SIZE_T_MAX )
				{
					continue;
				}

				const DynamicString filePath = task.asset.inputFilePath.getDirectoryWithPrefix();
				const DynamicString fileName = task.asset.inputFilePath.getFilenameWithExtension();

				const DynamicString sql = formatDynamicString(
					"INSERT INTO assets (filename, path, type, has_error) VALUES ('%s', '%s', '%s', '1');",
					fileName.cStr(),
					filePath.cStr(),
					task.asset.type.cStr()
				);

				if( !m_dataBase.executeCommand( sql.cStr() ) )
				{
					TIKI_TRACE_ERROR( "[converter] SQL command failed. Error: %s\n", m_dataBase.getLastError() );
					result = false;
					continue;
				}

				task.asset.assetId			= m_dataBase.getLastInsertId();
				task.asset.isAlradyBuilt	= false;
			}
		}

		// delete old input files
		if( !whereAssetId.isEmpty() )
		{
			const DynamicString sql = formatDynamicString( "DELETE FROM input_files WHERE %s;", whereAssetId.cStr() );
			if( !m_dataBase.executeCommand( sql.cStr() ) )
			{
				TIKI_TRACE_ERROR( "[converter] SQL command failed. Error: %s\n", m_dataBase.getLastError() );
				return false;
			}
		}

		DynamicString insertInputFiles;
		for( uintreg taskIndex = 0u; taskIndex < tasks.getCount(); ++taskIndex )
		{
			ConversionTask& task = tasks[ taskIndex ];

			insertInputFiles += formatDynamicString(
				"%s('%u','%s','%s')",
				taskIndex == 0 ? "" : ",",
				task.asset.assetId,
				task.asset.inputFilePath.getCompletePath(),
				task.asset.type.cStr()
			);
		}

		const DynamicString sql = formatDynamicString( "INSERT INTO input_files (asset_id,filename,type) VALUES %s;", insertInputFiles.cStr() );
		if( !m_dataBase.executeCommand( sql.cStr() ) )
		{
			TIKI_TRACE_ERROR( "[converter] SQL command failed. Error: %s\n", m_dataBase.getLastError() );
			return false;
		}

		return true;
	}

	bool AssetConverter::checkDependencies( List< ConversionTask >& tasks )
	{
		if( m_isNewDatabase || m_rebuildForced )
		{
			for( ConversionTask& task : tasks )
			{
				task.asset.isAlradyBuilt = false;
			}

			return true;
		}

		DynamicString whereAssetId;
		Map< uintreg, ConversionTask* > tasksByAssetId;
		for( uintreg i = 0u; i < tasks.getCount(); ++i )
		{
			ConversionTask& task = tasks[ i ];

			tasksByAssetId.set( task.asset.assetId, &task );

			if( i != 0u )
			{
				whereAssetId += " OR ";
			}
			whereAssetId += formatDynamicString( "asset_id = '%u'", task.asset.assetId );
		}

		if( !whereAssetId.isEmpty() )
		{
			// check output files
			{
				const DynamicString sql = formatDynamicString( "SELECT asset_id, filename FROM output_files WHERE %s;", whereAssetId.cStr() );

				SqliteQuery query;
				if( !query.create( m_dataBase, sql.cStr() ) )
				{
					TIKI_TRACE_ERROR( "[converter] can't prepare sql command. error: %s\n", query.getLastError() );
					return false;
				}

				while( query.nextRow() )
				{
					const uintreg assetId = query.getIntegerField( "asset_id" );

					ConversionTask* pTask = nullptr;
					if( !tasksByAssetId.findValue( &pTask, assetId ) || pTask == nullptr )
					{
						continue;
					}

					if( !pTask->asset.isAlradyBuilt )
					{
						continue;
					}

					if( !file::exists( query.getTextField( "filename" ) ) )
					{
						pTask->asset.isAlradyBuilt = false;
					}
				}
			}

			// check dependency files
			{
				const DynamicString sql = formatDynamicString( "SELECT asset_id, type, identifier, value_int FROM dependencies WHERE %s;", whereAssetId.cStr() );

				SqliteQuery query;
				if( !query.create( m_dataBase, sql.cStr() ) )
				{
					TIKI_TRACE_ERROR( "[converter] can't prepare sql command. error: %s\n", query.getLastError() );
					return false;
				}

				while( query.nextRow() )
				{
					const uintreg assetId = query.getIntegerField( "asset_id" );

					ConversionTask* pTask = nullptr;
					if( !tasksByAssetId.findValue( &pTask, assetId ) || pTask == nullptr )
					{
						continue;
					}

					if( !pTask->asset.isAlradyBuilt )
					{
						continue;
					}

					const ConversionResult::DependencyType type = (ConversionResult::DependencyType)query.getIntegerField( "type" );
					const DynamicString identifier	= query.getTextField( "identifier" );
					const sint64 valueInt	= query.getBigIntField( "value_int" );

					switch( type )
					{
					case ConversionResult::DependencyType_Converter:
						{
							const crc32 typeCrc = crcString( pTask->asset.type );
							const uint32 converterRevision = pTask->pConverter->getConverterRevision( typeCrc );
							if( (uint32)valueInt != converterRevision || converterRevision == (uint32)-1 )
							{
								pTask->asset.isAlradyBuilt = false;
							}
						}
						break;

					case ConversionResult::DependencyType_InputFile:
						{
							const crc32 fileChangeCrc = file::getLastChangeCrc( identifier.cStr() );
							if( fileChangeCrc != (crc32)valueInt )
							{
								pTask->asset.isAlradyBuilt = false;
							}
						}
						break;

					case ConversionResult::DependencyType_OutputFile:
						{
							if( !file::exists( identifier.cStr() ) )
							{
								pTask->asset.isAlradyBuilt = false;
							}
						}
						break;

					case ConversionResult::DependencyType_Type:
						{
							// TODO
							pTask->asset.isAlradyBuilt = false;
						}
						break;

					}
				}
			}
		}

		return true;
	}

	bool AssetConverter::finalizeTasks( List<ConversionTask>& tasks )
	{
		DynamicString whereAssetId;
		DynamicString whereAssetIdFailed;
		DynamicString whereAssetIdSucceeded;
		DynamicString dependencyValues;
		DynamicString traceValues;
		DynamicString outputFileValues;

		bool hasGlobalError = false;
		for( uintreg taskIndex = 0u; taskIndex < tasks.getCount(); ++taskIndex )
		{
			const ConversionTask& task = tasks[ taskIndex ];

			// dependencies
			{
				const List< ConversionResult::Dependency >& dependencies = task.result.getDependencies();
				for( uintreg i = 0u; i < dependencies.getCount(); ++i )
				{
					const ConversionResult::Dependency& dependency = dependencies[ i ];

					if( !dependencyValues.isEmpty() )
					{
						dependencyValues += ", ";
					}

					dependencyValues += formatDynamicString(
						"('%u','%u','%s','%lld')",
						task.asset.assetId,
						dependency.type,
						dependency.identifier.cStr(),
						dependency.intValue
					);
				}
			}

			// traces
			bool hasError = false;
			{
				const List< ConversionResult::TraceInfo >& traceInfos = task.result.getTraceInfos();
				for( uintreg i = 0u; i < traceInfos.getCount(); ++i )
				{
					const ConversionResult::TraceInfo& traceInfo = traceInfos[ i ];

					if( !traceValues.isEmpty() )
					{
						traceValues += ", ";
					}

					traceValues += formatDynamicString(
						"('%u','%u','%s')",
						task.asset.assetId,
						traceInfo.level,
						escapeString( traceInfo.message ).cStr()
					);

					if( traceInfo.level == TraceLevel_Error )
					{
						hasError = true;
						hasGlobalError = true;
					}
				}
			}

			if( hasError )
			{
				TIKI_TRACE_ERROR( "[converter] Conversion of '%s' failed!\n", task.asset.inputFilePath.getFilenameWithExtension() );
			}

			// output files
			{
				const List< Path >& outputFiles = task.result.getOutputFiles();
				for( uintreg i = 0u; i < outputFiles.getCount(); ++i )
				{
					const Path& outputFilePath = outputFiles[ i ];

					if( !outputFileValues.isEmpty() )
					{
						outputFileValues += ", ";
					}

					outputFileValues += formatDynamicString(
						"('%u','%s')",
						task.asset.assetId,
						outputFilePath.getCompletePath()
					);

					if( !hasError )
					{
						MutexStackLock lock( m_changedFilesMutex );
						m_changedFiles.add( outputFilePath.getCompletePath() );
					}
				}
			}

			if( !whereAssetId.isEmpty() )
			{
				whereAssetId += " OR ";
			}
			whereAssetId += formatDynamicString( "asset_id = '%u'", task.asset.assetId );

			DynamicString& whereAssetIdFiltered = (hasError ? whereAssetIdFailed : whereAssetIdSucceeded);
			if( !whereAssetIdFiltered.isEmpty() )
			{
				whereAssetIdFiltered += " OR ";
			}
			whereAssetIdFiltered += formatDynamicString( "id = '%u'", task.asset.assetId );
		}

		// delete old stuff
		if( !whereAssetId.isEmpty() )
		{
			const DynamicString deleteCommand =
				"DELETE FROM dependencies WHERE " + whereAssetId + "; " +
				"DELETE FROM output_files WHERE " + whereAssetId + "; " +
				"DELETE FROM traces WHERE " + whereAssetId + ";";

			if( !m_dataBase.executeCommand( deleteCommand.cStr() ) )
			{
				TIKI_TRACE_ERROR( "[converter] SQL command failed. Error: %s\n", m_dataBase.getLastError() );
				hasGlobalError = true;
			}
		}

		// dependencies
		if( !dependencyValues.isEmpty() )
		{
			const DynamicString sql = formatDynamicString( "INSERT INTO dependencies (asset_id,type,identifier,value_int) VALUES %s;", dependencyValues.cStr() );
			if( !m_dataBase.executeCommand( sql.cStr() ) )
			{
				TIKI_TRACE_ERROR( "[converter] SQL command failed. Error: %s\n", m_dataBase.getLastError() );
				hasGlobalError = true;
			}
		}

		// traces
		if( !traceValues.isEmpty() )
		{
			const DynamicString sql = formatDynamicString( "INSERT INTO traces (asset_id,level,message) VALUES %s;", traceValues.cStr() );
			if( !m_dataBase.executeCommand( sql.cStr() ) )
			{
				TIKI_TRACE_ERROR( "[converter] SQL command failed. Error: %s\n", m_dataBase.getLastError() );
				hasGlobalError = true;
			}
		}

		// output files
		if( !outputFileValues.isEmpty() )
		{
			const DynamicString sql = formatDynamicString( "INSERT INTO output_files (asset_id,filename) VALUES %s;", outputFileValues.cStr() );
			if( !m_dataBase.executeCommand( sql.cStr() ) )
			{
				TIKI_TRACE_ERROR( "[converter] SQL command failed. Error: %s\n", m_dataBase.getLastError() );
				hasGlobalError = true;
			}
		}

		// update asset
		if( !whereAssetIdFailed.isEmpty() )
		{
			const DynamicString sql = formatDynamicString( "UPDATE assets SET has_error = '1' WHERE %s;", whereAssetIdFailed.cStr() );
			if( !m_dataBase.executeCommand( sql.cStr() ) )
			{
				TIKI_TRACE_ERROR( "[converter] SQL command failed. Error: %s\n", m_dataBase.getLastError() );
				hasGlobalError = true;
			}
		}

		if( !whereAssetIdSucceeded.isEmpty() )
		{
			const DynamicString sql = formatDynamicString( "UPDATE assets SET has_error = '0' WHERE %s;", whereAssetIdSucceeded.cStr() );
			if( !m_dataBase.executeCommand( sql.cStr() ) )
			{
				TIKI_TRACE_ERROR( "[converter] SQL command failed. Error: %s\n", m_dataBase.getLastError() );
				hasGlobalError = true;
			}
		}

		{
			const DynamicString sql = formatDynamicString( "INSERT INTO builds (buildtime, has_error) VALUES (datetime('now'), %d);", hasGlobalError );
			if( !m_dataBase.executeCommand( sql.cStr() ) )
			{
				TIKI_TRACE_ERROR( "[converter] SQL command failed. Error: %s\n", m_dataBase.getLastError() );
				hasGlobalError = true;
			}
		}

		return !hasGlobalError;
	}

	/*static*/ void AssetConverter::taskConvertFile( const TaskContext& context )
	{
		TIKI_ASSERT( context.pTaskData != nullptr );
		ConversionTask& task = *static_cast< ConversionTask* >( context.pTaskData );

		task.pAssetConverter->taskRegisterResult( context.thread.getThreadId(), task.result );

		TIKI_TRACE_INFO( "Building asset: %s\n", task.asset.inputFilePath.getFilenameWithExtension() );
		task.pConverter->convert( task.result, task.asset, task.pAssetConverter->getContext() );

		task.pAssetConverter->taskUnregisterResult( context.thread.getThreadId() );
	}

	void AssetConverter::taskRegisterResult( uint64 threadId, ConversionResult& result )
	{
		m_loggingMutex.lock();
		m_loggingThreadResults.set( threadId, &result );
		m_loggingMutex.unlock();
	}

	void AssetConverter::taskUnregisterResult( uint64 threadId )
	{
		m_loggingMutex.lock();
		m_loggingThreadResults.set( threadId, nullptr );
		m_loggingMutex.unlock();
	}

	void AssetConverter::watcherThreadEntryPoint( const Thread& thread )
	{
		queueAll();

		while ( !thread.isExitRequested() )
		{
			FileWatcherEvent fileEvent;
			while( m_watcher.popEvent( fileEvent )  )
			{
				if( fileEvent.eventType != FileWatcherEventType_Modified )
				{
					continue;
				}

				MutexStackLock lock( m_queuedFilesMutex );

				bool found = false;
				for( uintreg i = 0u; i < m_queuedFiles.getCount(); ++i )
				{
					found |= (m_queuedFiles[ i ] == fileEvent.filePath);
				}

				if( !found )
				{
					m_queuedFiles.pushBack( fileEvent.filePath );
				}
			}

			bool start = false;
			{
				MutexStackLock lock( m_queuedFilesMutex );
				start = !m_queuedFiles.isEmpty();
			}

			if( start )
			{
				m_threadWork.setRelaxed( true );
				const bool ok = startConversion();
				m_threadSuccessful.setRelaxed( ok );
				m_threadFinish.setRelaxed( true );
				m_threadWork.setRelaxed( false );
			}

			Thread::sleepCurrentThread( 10u );
		}
	}

	int AssetConverter::watcherThreadStaticEntryPoint( const Thread& thread )
	{
		AssetConverter* pConverter = static_cast< AssetConverter* >( thread.getArgument() );
		pConverter->watcherThreadEntryPoint( thread );
		return 0u;
	}
}