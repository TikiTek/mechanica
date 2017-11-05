#include "tiki/converterbase/converter_manager.hpp"

#include "tiki/base/autodispose.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/path.hpp"
#include "tiki/base/platform.hpp"
#include "tiki/converterbase/converterbase.hpp"
#include "tiki/io/directory.hpp"
#include "tiki/io/file.hpp"
#include "tiki/io/path.hpp"
#include "tiki/io/xmlreader.hpp"
#include "tiki/tasksystem/taskcontext.hpp"
#include "tiki/toolxml/xml_attribute.hpp"
#include "tiki/toolxml/xml_document.hpp"
#include "tiki/toolxml/xml_element.hpp"

namespace tiki
{
	void globalTraceCallback( const char* message, TraceLevel level, UserData userData )
	{
		ConverterManager* pConverterManager = (ConverterManager*)userData.pContext;
		pConverterManager->traceCallback( message, level );
	}

	static string escapeString( const string& text )
	{
		return text.replace( "'", "''" ).replace( "\"", "\"\"" );
	}

	ConverterManager::ConverterManager()
	{
	}

	ConverterManager::~ConverterManager()
	{
		TIKI_ASSERT( m_converters.getCount() == 0u );
	}

	bool ConverterManager::create( const ConverterManagerParameter& parameters )
	{
		m_sourcePath		= parameters.sourcePath;
		m_outputPath		= parameters.outputPath;
		m_pChangedFilesList	= parameters.pChangedFilesList;
		m_rebuildForced		= parameters.forceRebuild;

		TaskSystemParameters taskParameters;
		taskParameters.maxTaskCount = 1024u;
		m_taskSystem.create( taskParameters );

		if ( !directory::exists( m_outputPath.cStr() ) )
		{
			directory::create( m_outputPath.cStr() );
		}

		const string databaseFileName = path::combine( m_outputPath, "build.sqlite" );
		m_isNewDatabase = !file::exists( databaseFileName.cStr() );
		if ( m_dataBase.create( databaseFileName.cStr() ) == false )
		{
			TIKI_TRACE_ERROR( "[convertermanager] database intialization failed.\n" );
			return false;
		}

		if ( m_isNewDatabase )
		{
			TIKI_TRACE_INFO( "[convertermanager] Database not found. Create new Database.\n" );

			const char* pCreateTableSql[] =
			{
				"CREATE TABLE builds (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, buildtime DATETIME, has_error BOOL);",
				"CREATE TABLE assets (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, filename TEXT NOT NULL, path TEXT NOT NULL, type INTEGER NOT NULL, has_error BOOL);",
				"CREATE TABLE dependencies (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, asset_id INTEGER NOT NULL, type INTEGER, identifier TEXT, value_int BIGINT);",
				"CREATE TABLE input_files (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, asset_id INTEGER NOT NULL, filename TEXT NOT NULL, type INTEGER NOT NULL);",
				"CREATE TABLE output_files (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, asset_id INTEGER NOT NULL, filename TEXT NOT NULL);",
				"CREATE TABLE traces (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, asset_id INTEGER NOT NULL, level INTEGER, message TEXT);"
			};

			for (uint i = 0u; i < TIKI_COUNT( pCreateTableSql ); ++i)
			{
				if ( m_dataBase.executeCommand( pCreateTableSql[ i ] ) == false )
				{
					TIKI_TRACE_ERROR( "[convertermanager] Could not create Table. Error: %s\n", m_dataBase.getLastError() );
					m_dataBase.dispose();
					return false;
				}
			}
		}
		else
		{
			SqliteQuery query;
			if ( query.create( m_dataBase, "SELECT * FROM builds WHERE has_error = 0 LIMIT 1;" ) )
			{
				m_isNewDatabase = !query.nextRow();
			}
			else
			{
				TIKI_TRACE_ERROR( "[convertermanager] Unable to read from builds table. Error: %s\n", m_dataBase.getLastError() );
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

		for( uint i = 0u; i < TIKI_COUNT( apConverters ); ++i )
		{
			ConverterBase* pConverter = apConverters[ i ];

			pConverter->create( m_outputPath, &m_taskSystem );
			registerConverter( pConverter );
		}

		addPackage( parameters.packageName );

		TIKI_TRACE_INFO( "ConverterManager: started\n" );
		return true;
	}

	void ConverterManager::dispose()
	{
		TIKI_TRACE_INFO( "ConverterManager: shutdown\n" );

		m_animationConverter.dispose();
		m_fontConverter.dispose();
		m_genericDataConverter.dispose();
		m_modelConverter.dispose();
		m_shaderConverter.dispose();
		m_textureConverter.dispose();

		debug::setTraceCallback( nullptr );

		m_taskSystem.dispose();

		m_loggingStream.dispose();
		m_loggingMutex.dispose();
		m_loggingThreadResults.dispose();

		m_dataBase.dispose();
	}

	void ConverterManager::queueFile( const Path& filePath )
	{
		m_files.add( filePath );
	}

	bool ConverterManager::startConversion( Mutex* pConversionMutex /* = nullptr */ )
	{
		if ( !prepareTasks() )
		{
			return false;
		}

		for (uint i = 0u; i < m_tasks.getCount(); ++i)
		{
			ConversionTask& task = m_tasks[ i ];
			task.taskId = queueTask( taskConvertFile, &task );
		}

		m_taskSystem.waitForAllTasks();

		return finalizeTasks();
	}

	void ConverterManager::registerConverter( const ConverterBase* pConverter )
	{
		m_converters.add( pConverter );
	}

	void ConverterManager::unregisterConverter( const ConverterBase* pConverter )
	{
		m_converters.removeSortedByValue( pConverter );
	}

	TaskId ConverterManager::queueTask( TaskFunc pFunc, void* pData, TaskId dependingTaskId /*= InvalidTaskId */ )
	{
		return m_taskSystem.queueTask( pFunc, pData, dependingTaskId );
	}

	void ConverterManager::waitForTask( TaskId taskId )
	{
		m_taskSystem.waitForTask( taskId );
	}

	void ConverterManager::traceCallback( const char* message, TraceLevel level )
	{
		string line = message;
		if ( !line.endsWith('\n') )
		{
			line += "\n";
		}

		const uint64 currentThreadId = Thread::getCurrentThreadId();

		m_loggingMutex.lock();

		m_loggingStream.write( line.cStr(), line.getLength() );

		ConversionResult* pResult = nullptr;
		if ( m_loggingThreadResults.findValue( &pResult, currentThreadId ) && pResult != nullptr )
		{
			pResult->addTraceInfo( level, message );
		}

		m_loggingMutex.unlock();
	}

	void ConverterManager::addPackage( const string& packageName )
	{
		Path definitionFilename;
		definitionFilename.setCombinedPath( m_sourcePath.cStr(), packageName.cStr() );
		definitionFilename.setExtension( ".package" );

		XmlDocument document;
		if( !document.loadFromFile( definitionFilename.getCompletePath() ) )
		{
			TIKI_TRACE_ERROR( "[converter] failed to load package definition from '%s'.\n", definitionFilename.getCompletePath() );
			return;
		}

		const XmlElement* pPackageNode = document.findFirstChild( "package" );
		if( pPackageNode == nullptr )
		{
			TIKI_TRACE_ERROR( "[converter] failed to find root node in package definition from '%s'.\n", definitionFilename.getCompletePath() );
			return;
		}


	}

	void ConverterManager::addTemplate( const Path& filePath )
	{
		XmlDocument document;
		if( !document.loadFromFile( filePath.getCompletePath() ) )
		{
			return;
		}

		const XmlElement* pRoot = document.findFirstChild( "template" );
		if( pRoot == nullptr )
		{
			TIKI_TRACE_ERROR( "[convertermanager] can't find 'template' root node in '%s'.\n", filePath.getCompletePath() );
			return;
		}

		const XmlAttribute* pAttName = pRoot->findAttribute( "name" );
		if( pAttName == nullptr )
		{
			TIKI_TRACE_ERROR( "[convertermanager] name argument not found. can't add template: %s\n", filePath.getCompletePath() );
			return;
		}

		TemplateDescription desc;
		desc.name		= pAttName->getValue();
		desc.filePath	= filePath;

		const XmlElement* pParameterNode = pRoot->findFirstChild( "parameter" );
		while( pParameterNode )
		{
			const XmlAttribute* pAttKey		= pParameterNode->findAttribute( "key" );
			const XmlAttribute* pAttValue	= pParameterNode->findAttribute( "value" );

			if( pAttKey == nullptr )
			{
				TIKI_TRACE_WARNING(
					"param failed: %s%s\n",
					(pAttKey == nullptr ? "no key-attribute " : string( "key: " ) + pAttKey->getValue()).cStr(),
					(pAttValue == nullptr ? "no value-attribute " : string( "value: " ) + pAttValue->getValue()).cStr()
				);
			}
			else if( pAttValue == nullptr )
			{
				desc.parameters.getMap().set( pAttKey->getValue(), pParameterNode->getValue() );
			}
			else
			{
				desc.parameters.getMap().set( pAttKey->getValue(), pAttValue->getValue() );
			}

			pParameterNode = pParameterNode->findNextSibling( "parameter" );
		}

		m_templates.set( desc.name, desc );
	}

	bool ConverterManager::prepareTasks()
	{
		List< ConversionAsset > assetsToBuild;
		List< Path > filesFromDependencies;

		for (uint i = 0u; i < m_files.getCount(); ++i)
		{
			const Path& file = m_files[ i ];

			ConversionAsset& asset = assetsToBuild.pushBack();
			if( !fillAssetFromFilePath( asset, file ) )
			{
				assetsToBuild.popBack();
				filesFromDependencies.add( file );
			}
		}

		if ( filesFromDependencies.getCount() > 0u )
		{
			string whereFileName;
			for (uint i = 0u; i < filesFromDependencies.getCount(); ++i)
			{
				const Path& filePath = filesFromDependencies[ i ];

				if (i != 0u)
				{
					whereFileName += " OR ";
				}

				whereFileName += "dep.identifier = '";
				whereFileName += filePath.getCompletePath();
				whereFileName += "'";
			}

			{
				const string sql = formatDynamicString( "SELECT asset.* FROM dependencies as dep, assets as asset WHERE dep.type = '%u' AND asset.id = dep.asset_id AND (%s)", ConversionResult::DependencyType_InputFile, whereFileName.cStr() );

				SqliteQuery query;
				if ( !query.create( m_dataBase, sql.cStr() ) )
				{
					TIKI_TRACE_ERROR( "[convertermanager] SQL command failed. Error: %s\n", query.getLastError() );
					return false;
				}

				while ( query.nextRow() )
				{
					ConversionAsset asset;
					asset.inputFilePath.setCombinedPath( query.getTextField( "path" ), query.getTextField( "filename" ) );
					asset.assetName	= asset.inputFilePath.getFilename();
					asset.typeCrc	= (crc32)query.getIntegerField( "type" );

					bool found = false;
					for (uint i = 0u; i < assetsToBuild.getCount(); ++i)
					{
						if( assetsToBuild[ i ].assetName == asset.assetName )
						{
							found = true;
							break;
						}
					}

					if ( !found )
					{
						assetsToBuild.add( asset );
					}
				}
			}
		}

		m_files.clear();
		m_tasks.clear();

		return generateTaskFromFiles( assetsToBuild );
	}

	bool ConverterManager::fillAssetFromFilePath( ConversionAsset& asset, const Path& filePath )
	{
		asset.inputFilePath = filePath;
		asset.assetName		= asset.inputFilePath.getFilename();
		//asset.typeCrc	= (crc32)query.getIntegerField( "type" );

		return true;
	}

	bool ConverterManager::generateTaskFromFiles( const List< ConversionAsset >& assetsToBuild )
	{
		List< ConversionTask > tasks;

		bool result = true;
		for (uint assetIndex = 0u; assetIndex < assetsToBuild.getCount(); ++assetIndex )
		{
			const ConversionAsset& asset = assetsToBuild[ assetIndex ];

			if ( !file::exists( asset.inputFilePath.getCompletePath() ) )
			{
				TIKI_TRACE_ERROR( "[converter] File to convert not found: %s.\n", asset.inputFilePath.getCompletePath() );
				result = false;
				continue;
			}

			ConversionTask task;
			for (size_t converterIndex = 0u; converterIndex < m_converters.getCount(); ++converterIndex )
			{
				const ConverterBase* pConverter = m_converters[ converterIndex ];

				if ( pConverter->canConvertType( asset.typeCrc ) )
				{
					task.pConverter = pConverter;
					break;
				}
			}

			if ( task.pConverter == nullptr )
			{
				TIKI_TRACE_ERROR( "[converter] No Converter found for file: '%s'.\n", asset.inputFilePath.getCompletePath() );
				result = false;
				continue;
			}

			task.asset					= asset;
			task.asset.isBuildRequired	= false;
			task.pManager				= this;

			task.result.addDependency( ConversionResult::DependencyType_Converter, "", string_tools::toString( task.pConverter->getConverterRevision( asset.typeCrc ) ) );
			task.result.addInputFile( task.asset.inputFilePath );

			tasks.add( task );
		}

		if ( !writeConvertInputs( tasks ) )
		{
			return false;
		}

		if ( !checkDependencies( tasks ) )
		{
			return false;
		}

		for (uint i = 0u; i < tasks.getCount(); ++i)
		{
			ConversionTask& task = tasks[ i ];

			if ( task.asset.isBuildRequired )
			{
				m_tasks.add( task );
			}
		}

		return result;
	}

	//bool ConverterManager::readDataFromXasset( ConversionTask& task, const FileDescription& fileDesc )
	//{
	//	XmlReader xmlFile;
	//	xmlFile.create( fileDesc.fullFileName.cStr() );

	//	// parse root node
	//	const XmlElement* pRoot = xmlFile.findNodeByName( "tikiasset" );
	//	if ( pRoot == nullptr )
	//	{
	//		TIKI_TRACE_ERROR( "no asset definition found.\n" );
	//		return false;
	//	}

	//	const XmlAttribute* pOutput = xmlFile.findAttributeByName( "output-name", pRoot );
	//	if ( pOutput == nullptr )
	//	{
	//		task.parameters.outputName = path::getFilenameWithoutExtension( path::getFilenameWithoutExtension( fileDesc.fullFileName ) );
	//	}
	//	else
	//	{
	//		task.parameters.outputName = pOutput->content;
	//	}

	//	// read inputs
	//	const string inputDir		= path::getDirectoryName( fileDesc.fullFileName );
	//	const XmlElement* pInput	= xmlFile.findNodeByName( "input" );
	//	while ( pInput != nullptr )
	//	{
	//		const XmlAttribute* pAttFile	= xmlFile.findAttributeByName( "file", pInput );
	//		const XmlAttribute* pAttType	= xmlFile.findAttributeByName( "type", pInput );

	//		if ( pAttFile == nullptr || pAttType == nullptr )
	//		{
	//			TIKI_TRACE_WARNING(
	//				"[ConverterManager] XASSET: Input-Tag has wrong attributes: %s%s\n",
	//				( pAttFile == nullptr ? "no file-attribute " : string( "file: " ) + pAttFile->content ).cStr(),
	//				( pAttType == nullptr ? "no type-attribute " : string( "type: " ) + pAttType->content ).cStr()
	//			);
	//		}
	//		else
	//		{
	//			ConversionParameters::InputFile input;
	//			input.fileName	= pAttFile->content;
	//			input.typeCrc	= crcString( pAttType->content );

	//			if ( !file::exists( input.fileName.cStr() ) )
	//			{
	//				input.fileName = path::combine( inputDir, input.fileName );
	//			}

	//			if ( !file::exists( input.fileName.cStr() ) )
	//			{
	//				TIKI_TRACE_WARNING( "input file not found. these will be ignored. path: %s\n", input.fileName.cStr() );
	//			}
	//			else
	//			{
	//				input.fileName = path::getAbsolutePath( input.fileName );
	//				task.parameters.inputFiles.add( input );
	//			}
	//		}

	//		pInput = xmlFile.findNext( "input", pInput );
	//	}

	//	if ( task.parameters.inputFiles.getCount() == 0u )
	//	{
	//		TIKI_TRACE_ERROR( "no inputs specified. asset can't be converted. Filename: %s\n", task.parameters.sourceFile.cStr() );
	//		xmlFile.dispose();
	//		return false;
	//	}

	//	// read params
	//	parseParams( xmlFile, pRoot, task.parameters.arguments.getMap() );

	//	const XmlAttribute* pTemplate = xmlFile.findAttributeByName( "template", pRoot );
	//	if ( pTemplate != nullptr )
	//	{
	//		TemplateDescription desc;
	//		if ( m_templates.findValue( &desc, pTemplate->content ) )
	//		{
	//			for (uint i = 0u; i < desc.arguments.getCount(); ++i)
	//			{
	//				KeyValuePair< string, string >& kvp = desc.arguments.getPairAt( i );

	//				task.parameters.arguments.getMap().set( kvp.key, kvp.value );
	//			}
	//		}
	//	}
	//	xmlFile.dispose();

	//	return true;
	//}

	bool ConverterManager::writeConvertInputs( List< ConversionTask >& tasks )
	{
		if ( tasks.isEmpty() )
		{
			return true;
		}

		bool result = true;

		string whereFileName;
		Map< string, ConversionTask* > tasksByFileName;
		for (uint i = 0u; i < tasks.getCount(); ++i)
		{
			ConversionTask& task = tasks[ i ];
			const string fileName = task.asset.inputFilePath.getFilenameWithExtension();

			if ( i != 0u )
			{
				whereFileName += " OR ";
			}

			whereFileName += "filename = '" + fileName + "'";

			tasksByFileName.set( fileName, &task );
		}

		// read asset ids
		string whereAssetId;
		{
			const string sql = "SELECT id, filename, has_error FROM assets WHERE " + whereFileName;

			SqliteQuery query;
			if ( !query.create( m_dataBase, sql.cStr() ) )
			{
				TIKI_TRACE_ERROR( "[convertermanager] SQL command failed. Error: %s\n", m_dataBase.getLastError() );
				return false;
			}

			while ( query.nextRow() )
			{
				const string fileName = query.getTextField( "filename" );

				ConversionTask* pTask = nullptr;
				if ( tasksByFileName.findValue( &pTask, fileName ) && pTask != nullptr )
				{
					pTask->asset.assetId			= query.getIntegerField( "id" );
					pTask->asset.isBuildRequired	= (query.getIntegerField( "has_error" ) != 0u);

					if ( !whereAssetId.isEmpty() )
					{
						whereAssetId += " OR ";
					}
					whereAssetId += formatDynamicString( "asset_id = '%u'", pTask->asset.assetId );
				}
			}

			for (uint i = 0u; i < tasks.getCount(); ++i)
			{
				ConversionTask& task = tasks[ i ];

				if ( task.asset.assetId == TIKI_SIZE_T_MAX )
				{
					const string filePath = task.asset.inputFilePath.getDirectoryWithPrefix();
					const string fileName = task.asset.inputFilePath.getFilenameWithExtension();

					const string sql = formatDynamicString(
						"INSERT INTO assets (filename, path, type, has_error) VALUES ('%s', '%s', '%u', '1');",
						fileName.cStr(),
						filePath.cStr(),
						task.asset.typeCrc
					);

					if ( !m_dataBase.executeCommand( sql.cStr() ) )
					{
						TIKI_TRACE_ERROR( "[convertermanager] SQL command failed. Error: %s\n", m_dataBase.getLastError() );
						result = false;
						continue;
					}

					task.asset.assetId			= m_dataBase.getLastInsertId();
					task.asset.isBuildRequired	= true;
				}
			}
		}

		// delete old input files
		if ( !whereAssetId.isEmpty() )
		{
			const string sql = formatDynamicString( "DELETE FROM input_files WHERE %s;", whereAssetId.cStr() );
			if ( !m_dataBase.executeCommand( sql.cStr() ) )
			{
				TIKI_TRACE_ERROR( "[convertermanager] SQL command failed. Error: %s\n", m_dataBase.getLastError() );
				return false;
			}
		}

		string insertInputFiles;
		for (uint taskIndex = 0u; taskIndex < tasks.getCount(); ++taskIndex)
		{
			ConversionTask& task = tasks[ taskIndex ];

			insertInputFiles += formatDynamicString(
				"('%u','%s','%u')",
				task.asset.assetId,
				task.asset.inputFilePath.getCompletePath(),
				task.asset.typeCrc
			);
		}

		const string sql = formatDynamicString( "INSERT INTO input_files (asset_id,filename,type) VALUES %s;", insertInputFiles.cStr() );
		if ( !m_dataBase.executeCommand( sql.cStr() ) )
		{
			TIKI_TRACE_ERROR( "[convertermanager] SQL command failed. Error: %s\n", m_dataBase.getLastError() );
			return false;
		}

		return true;
	}

	bool ConverterManager::checkDependencies( List< ConversionTask >& tasks )
	{
		if ( m_isNewDatabase || m_rebuildForced )
		{
			for( ConversionTask& task : tasks )
			{
				task.asset.isBuildRequired = true;
			}

			return true;
		}

		string whereAssetId;
		Map< uint, ConversionTask* > tasksByAssetId;
		for (uint i = 0u; i < tasks.getCount(); ++i)
		{
			ConversionTask& task = tasks[ i ];

			tasksByAssetId.set( task.asset.assetId, &task );

			if ( i != 0u )
			{
				whereAssetId += " OR ";
			}
			whereAssetId += formatDynamicString( "asset_id = '%u'", task.asset.assetId );
		}

		if( !whereAssetId.isEmpty() )
		{
			// check output files
			{
				const string sql = formatDynamicString( "SELECT asset_id, filename FROM output_files WHERE %s;", whereAssetId.cStr() );

				SqliteQuery query;
				if( !query.create( m_dataBase, sql.cStr() ) )
				{
					TIKI_TRACE_ERROR( "[convertermanager] can't prepare sql command. error: %s\n", query.getLastError() );
					return false;
				}

				while( query.nextRow() )
				{
					const uint assetId = query.getIntegerField( "asset_id" );

					ConversionTask* pTask = nullptr;
					if( !tasksByAssetId.findValue( &pTask, assetId ) || pTask == nullptr )
					{
						continue;
					}

					if( pTask->asset.isBuildRequired )
					{
						continue;
					}

					if( !file::exists( query.getTextField( "filename" ) ) )
					{
						pTask->asset.isBuildRequired = true;
					}
				}
			}

			// check dependency files
			{
				const string sql = formatDynamicString( "SELECT asset_id, type, identifier, value_int FROM dependencies WHERE %s;", whereAssetId.cStr() );

				SqliteQuery query;
				if( !query.create( m_dataBase, sql.cStr() ) )
				{
					TIKI_TRACE_ERROR( "[convertermanager] can't prepare sql command. error: %s\n", query.getLastError() );
					return false;
				}

				while( query.nextRow() )
				{
					const uint assetId = query.getIntegerField( "asset_id" );

					ConversionTask* pTask = nullptr;
					if( !tasksByAssetId.findValue( &pTask, assetId ) || pTask == nullptr )
					{
						continue;
					}

					if( pTask->asset.isBuildRequired )
					{
						continue;
					}

					const ConversionResult::DependencyType type = (ConversionResult::DependencyType)query.getIntegerField( "type" );
					const string identifier	= query.getTextField( "identifier" );
					const int valueInt		= query.getIntegerField( "value_int" );

					switch( type )
					{
					case ConversionResult::DependencyType_Converter:
						{
							const uint32 converterRevision = pTask->pConverter->getConverterRevision( pTask->asset.typeCrc );
							if( (uint32)valueInt != converterRevision || converterRevision == (uint32)-1 )
							{
								pTask->asset.isBuildRequired = true;
							}
						}
						break;

					case ConversionResult::DependencyType_InputFile:
					case ConversionResult::DependencyType_OutputFile:
						{
							const crc32 fileChangeCrc = file::getLastChangeCrc( identifier.cStr() );
							if( fileChangeCrc != (crc32)valueInt )
							{
								pTask->asset.isBuildRequired = true;
							}
						}
						break;

					case ConversionResult::DependencyType_Type:
						{
							// TODO
							pTask->asset.isBuildRequired = true;
						}
						break;

					}
				}
			}
		}

		return true;
	}

	bool ConverterManager::finalizeTasks()
	{
		string whereAssetId;
		string whereAssetIdFailed;
		string whereAssetIdSucceeded;
		string dependencyValues;
		string traceValues;
		string outputFileValues;

		bool hasGlobalError = false;
		for (uint taskIndex = 0u; taskIndex < m_tasks.getCount(); ++taskIndex)
		{
			const ConversionTask& task = m_tasks[ taskIndex ];

			// dependencies
			{
				const List< ConversionResult::Dependency >& dependencies = task.result.getDependencies();
				for (uint i = 0u; i < dependencies.getCount(); ++i)
				{
					const ConversionResult::Dependency& dependency = dependencies[ i ];

					if( !dependencyValues.isEmpty() )
					{
						dependencyValues += ", ";
					}

					dependencyValues += formatDynamicString(
						"('%u','%u','%s','%s')",
						task.asset.assetId,
						dependency.type,
						dependency.identifier.cStr(),
						dependency.value.cStr()
					);
				}
			}

			// traces
			bool hasError = false;
			{
				const List< ConversionResult::TraceInfo >& traceInfos = task.result.getTraceInfos();
				for (uint i = 0u; i < traceInfos.getCount(); ++i)
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

					if ( traceInfo.level >= TraceLevel_Warning )
					{
						hasError = true;
						hasGlobalError = true;
					}
				}
			}

			if ( hasError )
			{
				TIKI_TRACE_ERROR( "[ConverterManager] Conversion of '%s' failed!\n", task.asset.inputFilePath.getFilenameWithExtension() );
			}

			// output files
			{
				const List< Path >& outputFiles = task.result.getOutputFiles();
				for (uint i = 0u; i < outputFiles.getCount(); ++i)
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

					if( m_pChangedFilesList != nullptr && !hasError )
					{
						m_pChangedFilesList->add( outputFilePath.getCompletePath() );
					}
				}
			}

			if( !whereAssetId.isEmpty() )
			{
				whereAssetId += " OR ";
			}
			whereAssetId += formatDynamicString( "asset_id = '%u'", task.asset.assetId );

			string& whereAssetIdFiltered = (hasError ? whereAssetIdFailed : whereAssetIdSucceeded);
			if( !whereAssetIdFiltered.isEmpty() )
			{
				whereAssetIdFiltered += " OR ";
			}
			whereAssetIdFiltered += formatDynamicString( "id = '%u'", task.asset.assetId );
		}

		// delete old stuff
		if ( !whereAssetId.isEmpty() )
		{
			const string deleteCommand =
				"DELETE FROM dependencies WHERE " + whereAssetId + "; " +
				"DELETE FROM output_files WHERE " + whereAssetId + "; " +
				"DELETE FROM traces WHERE " + whereAssetId + ";";

			if ( !m_dataBase.executeCommand( deleteCommand.cStr() ) )
			{
				TIKI_TRACE_ERROR( "[convertermanager] SQL command failed. Error: %s\n", m_dataBase.getLastError() );
				hasGlobalError = true;
			}
		}

		// dependencies
		if ( !dependencyValues.isEmpty() )
		{
			const string sql = formatDynamicString( "INSERT INTO dependencies (asset_id,type,identifier,value_int) VALUES %s;", dependencyValues.cStr() );
			if ( !m_dataBase.executeCommand( sql.cStr() ) )
			{
				TIKI_TRACE_ERROR( "[convertermanager] SQL command failed. Error: %s\n", m_dataBase.getLastError() );
				hasGlobalError = true;
			}
		}

		// traces
		if ( !traceValues.isEmpty() )
		{
			const string sql = formatDynamicString( "INSERT INTO traces (asset_id,level,message) VALUES %s;", traceValues.cStr() );
			if ( !m_dataBase.executeCommand( sql.cStr() ) )
			{
				TIKI_TRACE_ERROR( "[convertermanager] SQL command failed. Error: %s\n", m_dataBase.getLastError() );
				hasGlobalError = true;
			}
		}

		// output files
		if ( !outputFileValues.isEmpty() )
		{
			const string sql = formatDynamicString( "INSERT INTO output_files (asset_id,filename) VALUES %s;", outputFileValues.cStr() );
			if ( !m_dataBase.executeCommand( sql.cStr() ) )
			{
				TIKI_TRACE_ERROR( "[convertermanager] SQL command failed. Error: %s\n", m_dataBase.getLastError() );
				hasGlobalError = true;
			}
		}

		// update asset
		if ( !whereAssetIdFailed.isEmpty() )
		{
			const string sql = formatDynamicString( "UPDATE assets SET has_error = '1' WHERE %s;", whereAssetIdFailed.cStr() );
			if ( !m_dataBase.executeCommand( sql.cStr() ) )
			{
				TIKI_TRACE_ERROR( "[convertermanager] SQL command failed. Error: %s\n", m_dataBase.getLastError() );
				hasGlobalError = true;
			}
		}

		if ( !whereAssetIdSucceeded.isEmpty() )
		{
			const string sql = formatDynamicString( "UPDATE assets SET has_error = '0' WHERE %s;", whereAssetIdSucceeded.cStr() );
			if ( !m_dataBase.executeCommand( sql.cStr() ) )
			{
				TIKI_TRACE_ERROR( "[convertermanager] SQL command failed. Error: %s\n", m_dataBase.getLastError() );
				hasGlobalError = true;
			}
		}

		{
			const string sql = formatDynamicString( "INSERT INTO builds (buildtime, has_error) VALUES (datetime('now'), %d);", hasGlobalError );
			if ( !m_dataBase.executeCommand( sql.cStr() ) )
			{
				TIKI_TRACE_ERROR( "[convertermanager] SQL command failed. Error: %s\n", m_dataBase.getLastError() );
				hasGlobalError = true;
			}
		}

		return !hasGlobalError;
	}

	/*static*/ void ConverterManager::taskConvertFile( const TaskContext& context )
	{
		TIKI_ASSERT( context.pTaskData != nullptr );
		ConversionTask& task = *static_cast< ConversionTask* >( context.pTaskData );

		task.pManager->taskRegisterResult( context.thread.getThreadId(), task.result );

		TIKI_TRACE_INFO( "Building asset: %s\n", task.asset.inputFilePath.getFilenameWithExtension() );
		task.pConverter->convert( task.result, task.asset );
	}

	void ConverterManager::taskRegisterResult( uint64 threadId, ConversionResult& result )
	{
		m_loggingMutex.lock();
		m_loggingThreadResults.set( threadId, &result );
		m_loggingMutex.unlock();
	}

	void ConverterManager::taskUnregisterResult( uint64 threadId )
	{
		m_loggingMutex.lock();
		m_loggingThreadResults.set( threadId, nullptr );
		m_loggingMutex.unlock();
	}
}
