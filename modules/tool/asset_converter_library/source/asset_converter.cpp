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

	static string escapeString( const string& text )
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
		m_changedFilesMutex.create();

		if( m_pProject == nullptr )
		{
			m_pProject = TIKI_NEW( Project );
			m_ownsProject = true;
		}

		m_context.pProject = m_pProject;

		TaskSystemParameters taskParameters;
		taskParameters.maxTaskCount = 1024u;
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

			for( uint i = 0u; i < TIKI_COUNT( pCreateTableSql ); ++i )
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

		if( !m_genericDataConverter.setProject( m_pProject ) )
		{
			TIKI_TRACE_ERROR( "AssetConverter: Could not load generic data types!\n" );
			return false;
		}

		for( uint i = 0u; i < TIKI_COUNT( apConverters ); ++i )
		{
			ConverterBase* pConverter = apConverters[ i ];

			pConverter->create( m_pProject->getAssetBuildPath(), &m_taskSystem );

			List< string > extensions;
			pConverter->getInputExtensions( extensions );
			for( const string& extenstion : extensions )
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
			if( !convertAll() )
			{
				TIKI_TRACE_ERROR( "AssetConverter: Initial Asset conversion failed. Shutting down!\n" );
				dispose();
				return false;
			}
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

		for( uint i = 0u; i < TIKI_COUNT( apConverters ); ++i )
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
	}

	bool AssetConverter::convertAll()
	{
		List< Path > assetFiles;
		for( const KeyValuePair< string, string >& extension : m_extensions )
		{
			directory::findFiles( assetFiles, m_pProject->getContentPath(), extension.key );
		}

		for( const Path file : assetFiles )
		{
			queueFile( file );
		}
		TIKI_TRACE_INFO( "[converter] Complete scan finish! %u elements found.\n", assetFiles.getCount() );

		const bool result = startConversion( nullptr );
		TIKI_TRACE_INFO( "[converter] Conversion %s!\n", result ? "successful" : "failed" );

		return result;
	}

	void AssetConverter::startWatch()
	{
		m_watcherMutex.create();
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
		m_watcherMutex.dispose();
	}

	bool AssetConverter::getChangedFiles( Array< string >& changedFiles )
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

	void AssetConverter::queueFile( const Path& filePath )
	{
		m_files.add( filePath );
	}

	bool AssetConverter::startConversion( Mutex* pConversionMutex /* = nullptr */ )
	{
		if( !prepareTasks() )
		{
			return false;
		}

		for( uint i = 0u; i < m_tasks.getCount(); ++i )
		{
			ConversionTask& task = m_tasks[ i ];
			task.taskId = m_taskSystem.queueTask( taskConvertFile, &task );
		}

		m_taskSystem.waitForAllTasks();

		return finalizeTasks();
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
		string line = message;
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
		desc.name		= filePath.getFilename();
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
					(pAttKey == nullptr ? "no key-attribute " : string( "key: " ) + pAttKey->getValue()).cStr(),
					(pAttValue == nullptr ? "no value-attribute " : string( "value: " ) + pAttValue->getValue()).cStr()
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

	bool AssetConverter::prepareTasks()
	{
		List< ConversionAsset > assetsToBuild;
		List< Path > filesFromDependencies;

		for( uint i = 0u; i < m_files.getCount(); ++i )
		{
			const Path& file = m_files[ i ];

			ConversionAsset& asset = assetsToBuild.pushBack();
			if( !fillAssetFromFilePath( asset, file ) )
			{
				assetsToBuild.popBack();
				filesFromDependencies.add( file );
			}
		}

		if( filesFromDependencies.getCount() > 0u )
		{
			string whereFileName;
			for( uint i = 0u; i < filesFromDependencies.getCount(); ++i )
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
				const string sql = formatDynamicString( "SELECT asset.* FROM dependencies as dep, assets as asset WHERE dep.type = '%u' AND asset.id = dep.asset_id AND (%s)", ConversionResult::DependencyType_InputFile, whereFileName.cStr() );

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
					asset.assetName	= asset.inputFilePath.getFilename();
					asset.type		= query.getTextField( "type" );

					bool found = false;
					for( uint i = 0u; i < assetsToBuild.getCount(); ++i )
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

		m_files.clear();
		m_tasks.clear();

		return generateTaskFromFiles( assetsToBuild );
	}

	bool AssetConverter::fillAssetFromFilePath( ConversionAsset& asset, const Path& filePath )
	{
		Path assetPath;
		assetPath.setCompletePath( filePath.getFilename() );

		asset.inputFilePath = filePath;
		asset.assetName		= assetPath.getFilename();

		if( !isStringEmpty( assetPath.getExtension() ) )
		{
			const string templateName = assetPath.getExtension() + 1u;
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

	bool AssetConverter::generateTaskFromFiles( const List< ConversionAsset >& assetsToBuild )
	{
		List< ConversionTask > tasks;

		bool result = true;
		for( uint assetIndex = 0u; assetIndex < assetsToBuild.getCount(); ++assetIndex )
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
			task.asset.isBuildRequired	= false;
			task.pAssetConverter		= this;

			task.result.addDependency( ConversionResult::DependencyType_Converter, "", string_tools::toString( task.pConverter->getConverterRevision( typeCrc ) ) );
			task.result.addInputFile( task.asset.inputFilePath );

			tasks.add( task );
		}

		if( !writeConvertInputs( tasks ) )
		{
			return false;
		}

		if( !checkDependencies( tasks ) )
		{
			return false;
		}

		for( uint i = 0u; i < tasks.getCount(); ++i )
		{
			ConversionTask& task = tasks[ i ];

			if( task.asset.isBuildRequired )
			{
				m_tasks.add( task );
			}
		}

		return result;
	}

	//bool AssetConverter::readDataFromXasset( ConversionTask& task, const FileDescription& fileDesc )
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
	//				"[converter] XASSET: Input-Tag has wrong attributes: %s%s\n",
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

	bool AssetConverter::writeConvertInputs( List< ConversionTask >& tasks )
	{
		if( tasks.isEmpty() )
		{
			return true;
		}

		bool result = true;

		string whereFileName;
		Map< string, ConversionTask* > tasksByFileName;
		for( uint i = 0u; i < tasks.getCount(); ++i )
		{
			ConversionTask& task = tasks[ i ];
			const string fileName = task.asset.inputFilePath.getFilenameWithExtension();

			if( i != 0u )
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
			if( !query.create( m_dataBase, sql.cStr() ) )
			{
				TIKI_TRACE_ERROR( "[converter] SQL command failed. Error: %s\n", m_dataBase.getLastError() );
				return false;
			}

			while( query.nextRow() )
			{
				const string fileName = query.getTextField( "filename" );

				ConversionTask* pTask = nullptr;
				if( tasksByFileName.findValue( &pTask, fileName ) && pTask != nullptr )
				{
					pTask->asset.assetId			= query.getIntegerField( "id" );
					pTask->asset.isBuildRequired	= (query.getIntegerField( "has_error" ) != 0u);

					if( !whereAssetId.isEmpty() )
					{
						whereAssetId += " OR ";
					}
					whereAssetId += formatDynamicString( "asset_id = '%u'", pTask->asset.assetId );
				}
			}

			for( uint i = 0u; i < tasks.getCount(); ++i )
			{
				ConversionTask& task = tasks[ i ];

				if( task.asset.assetId == TIKI_SIZE_T_MAX )
				{
					const string filePath = task.asset.inputFilePath.getDirectoryWithPrefix();
					const string fileName = task.asset.inputFilePath.getFilenameWithExtension();

					const string sql = formatDynamicString(
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
					task.asset.isBuildRequired	= true;
				}
			}
		}

		// delete old input files
		if( !whereAssetId.isEmpty() )
		{
			const string sql = formatDynamicString( "DELETE FROM input_files WHERE %s;", whereAssetId.cStr() );
			if( !m_dataBase.executeCommand( sql.cStr() ) )
			{
				TIKI_TRACE_ERROR( "[converter] SQL command failed. Error: %s\n", m_dataBase.getLastError() );
				return false;
			}
		}

		string insertInputFiles;
		for( uint taskIndex = 0u; taskIndex < tasks.getCount(); ++taskIndex )
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

		const string sql = formatDynamicString( "INSERT INTO input_files (asset_id,filename,type) VALUES %s;", insertInputFiles.cStr() );
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
				task.asset.isBuildRequired = true;
			}

			return true;
		}

		string whereAssetId;
		Map< uint, ConversionTask* > tasksByAssetId;
		for( uint i = 0u; i < tasks.getCount(); ++i )
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
				const string sql = formatDynamicString( "SELECT asset_id, filename FROM output_files WHERE %s;", whereAssetId.cStr() );

				SqliteQuery query;
				if( !query.create( m_dataBase, sql.cStr() ) )
				{
					TIKI_TRACE_ERROR( "[converter] can't prepare sql command. error: %s\n", query.getLastError() );
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
					TIKI_TRACE_ERROR( "[converter] can't prepare sql command. error: %s\n", query.getLastError() );
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
							const crc32 typeCrc = crcString( pTask->asset.type );
							const uint32 converterRevision = pTask->pConverter->getConverterRevision( typeCrc );
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

	bool AssetConverter::finalizeTasks()
	{
		string whereAssetId;
		string whereAssetIdFailed;
		string whereAssetIdSucceeded;
		string dependencyValues;
		string traceValues;
		string outputFileValues;

		bool hasGlobalError = false;
		for( uint taskIndex = 0u; taskIndex < m_tasks.getCount(); ++taskIndex )
		{
			const ConversionTask& task = m_tasks[ taskIndex ];

			// dependencies
			{
				const List< ConversionResult::Dependency >& dependencies = task.result.getDependencies();
				for( uint i = 0u; i < dependencies.getCount(); ++i )
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
				for( uint i = 0u; i < traceInfos.getCount(); ++i )
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

					if( traceInfo.level >= TraceLevel_Warning )
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
				for( uint i = 0u; i < outputFiles.getCount(); ++i )
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

			string& whereAssetIdFiltered = (hasError ? whereAssetIdFailed : whereAssetIdSucceeded);
			if( !whereAssetIdFiltered.isEmpty() )
			{
				whereAssetIdFiltered += " OR ";
			}
			whereAssetIdFiltered += formatDynamicString( "id = '%u'", task.asset.assetId );
		}

		// delete old stuff
		if( !whereAssetId.isEmpty() )
		{
			const string deleteCommand =
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
			const string sql = formatDynamicString( "INSERT INTO dependencies (asset_id,type,identifier,value_int) VALUES %s;", dependencyValues.cStr() );
			if( !m_dataBase.executeCommand( sql.cStr() ) )
			{
				TIKI_TRACE_ERROR( "[converter] SQL command failed. Error: %s\n", m_dataBase.getLastError() );
				hasGlobalError = true;
			}
		}

		// traces
		if( !traceValues.isEmpty() )
		{
			const string sql = formatDynamicString( "INSERT INTO traces (asset_id,level,message) VALUES %s;", traceValues.cStr() );
			if( !m_dataBase.executeCommand( sql.cStr() ) )
			{
				TIKI_TRACE_ERROR( "[converter] SQL command failed. Error: %s\n", m_dataBase.getLastError() );
				hasGlobalError = true;
			}
		}

		// output files
		if( !outputFileValues.isEmpty() )
		{
			const string sql = formatDynamicString( "INSERT INTO output_files (asset_id,filename) VALUES %s;", outputFileValues.cStr() );
			if( !m_dataBase.executeCommand( sql.cStr() ) )
			{
				TIKI_TRACE_ERROR( "[converter] SQL command failed. Error: %s\n", m_dataBase.getLastError() );
				hasGlobalError = true;
			}
		}

		// update asset
		if( !whereAssetIdFailed.isEmpty() )
		{
			const string sql = formatDynamicString( "UPDATE assets SET has_error = '1' WHERE %s;", whereAssetIdFailed.cStr() );
			if( !m_dataBase.executeCommand( sql.cStr() ) )
			{
				TIKI_TRACE_ERROR( "[converter] SQL command failed. Error: %s\n", m_dataBase.getLastError() );
				hasGlobalError = true;
			}
		}

		if( !whereAssetIdSucceeded.isEmpty() )
		{
			const string sql = formatDynamicString( "UPDATE assets SET has_error = '0' WHERE %s;", whereAssetIdSucceeded.cStr() );
			if( !m_dataBase.executeCommand( sql.cStr() ) )
			{
				TIKI_TRACE_ERROR( "[converter] SQL command failed. Error: %s\n", m_dataBase.getLastError() );
				hasGlobalError = true;
			}
		}

		{
			const string sql = formatDynamicString( "INSERT INTO builds (buildtime, has_error) VALUES (datetime('now'), %d);", hasGlobalError );
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
		convertAll();

		while ( thread.isExitRequested() == false )
		{
			FileWatcherEvent fileEvent;
			if ( m_watcher.popEvent( fileEvent ) && fileEvent.eventType == FileWatcherEventType_Modified )
			{
				//MutexStackLock lock( m_converterMutex );

				//m_manager.queueFile( fileEvent.filePath );
				//m_manager.startConversion();
			}
			else
			{
				Thread::sleepCurrentThread( 10u );
			}
		}
	}

	int AssetConverter::watcherThreadStaticEntryPoint( const Thread& thread )
	{
		AssetConverter* pConverter = static_cast< AssetConverter* >( thread.getArgument() );
		pConverter->watcherThreadEntryPoint( thread );
		return 0u;
	}
}