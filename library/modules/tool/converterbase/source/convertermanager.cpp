
#include "tiki/converterbase/convertermanager.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/base/directory.hpp"
#include "tiki/base/file.hpp"
#include "tiki/base/iopath.hpp"
#include "tiki/base/platform.hpp"
#include "tiki/converterbase/conversionparameters.hpp"
#include "tiki/converterbase/converterbase.hpp"
#include "tiki/tasksystem/taskcontext.hpp"
#include "tiki/toolbase/autodispose.hpp"
#include "tiki/toolbase/tikixml.hpp"

#include "sqlite/sqlite3.h"

namespace tiki
{
	static ConverterManager*	s_pInstance			= nullptr;
	//static ConversionResult*	s_pCurrentResult	= nullptr;

	void globalTraceCallback( cstring message, TraceLevel level )
	{
		s_pInstance->traceCallback( message, level );

		//if ( s_pCurrentResult != nullptr )
		//{
		//	s_pCurrentResult->addTraceInfo( level, message );
		//}
	}

	static string escapeString( const string& text )
	{
		return text.replace( "'", "''" ).replace( "\"", "\"\"" );
	}

	ConverterManager::~ConverterManager()
	{
		TIKI_ASSERT( m_converters.getCount() == 0u );
	}

	void ConverterManager::create( const ConverterManagerParameter& parameters )
	{
		m_outputPath	= parameters.outputPath;
		m_rebuildForced	= parameters.forceRebuild;
		m_resourceMap.create( path::combine( m_outputPath, "resourcenamemap.rnm" ) );

		TaskSystemParameters taskParameters;		
		m_taskSystem.create( taskParameters );

		if ( !directory::exists( m_outputPath ) )
		{
			directory::create( m_outputPath );
		}

		const string databaseFileName = path::combine( m_outputPath, "build.sqlite" );
		m_isNewDatabase = !file::exists( databaseFileName );
		if ( m_dataBase.create( databaseFileName ) == false )
		{
			TIKI_TRACE_ERROR( "[convertermanager] database intialization failed.\n" );
		}

		if ( m_isNewDatabase )
		{
			TIKI_TRACE_INFO( "[convertermanager] Database not found. Create new Database.\n" );

			cstring pCreateTableSql[] =
			{
				"CREATE TABLE assets (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, filename TEXT NOT NULL, path TEXT NOT NULL, type INTEGER NOT NULL, has_error BOOL);",
				"CREATE TABLE dependencies (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, asset_id INTEGER NOT NULL, type INTEGER, identifier TEXT, value_text TEXT, value_int INTEGER);",
				"CREATE TABLE input_files (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, asset_id INTEGER NOT NULL, filename TEXT NOT NULL, type INTEGER NOT NULL);",
				"CREATE TABLE output_files (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, asset_id INTEGER NOT NULL, filename TEXT NOT NULL);",
				"CREATE TABLE traces (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, asset_id INTEGER NOT NULL, level INTEGER, message TEXT);"
			};

			for (uint i = 0u; i < TIKI_COUNT( pCreateTableSql ); ++i)
			{
				if ( m_dataBase.executeCommand( pCreateTableSql[ i ] ) == false )
				{
					TIKI_TRACE_ERROR( "[convertermanager] Could create Table. Error: %s\n", m_dataBase.getLastError().cStr() );
					m_dataBase.dispose();
					break;
				}
			}
		}

		m_loggingMutex.create();
		m_loggingStream.open( "converter.log", DataAccessMode_WriteAppend );

		s_pInstance = this;
		debug::setTraceCallback( globalTraceCallback );

		TIKI_TRACE_INFO( "ConverterManager: started\n" );
	}

	void ConverterManager::dispose()
	{
		TIKI_TRACE_INFO( "ConverterManager: shutdown\n" );
		
		debug::setTraceCallback( nullptr );

		m_taskSystem.dispose();

		m_resourceMap.writeToFile();
		m_resourceMap.dispose();

		m_loggingStream.close();
		m_loggingMutex.dispose();

		m_dataBase.dispose();
	}

	void ConverterManager::addTemplate( const string& fileName )
	{
		TikiXml xmlFile;
		xmlFile.create( fileName );

		const XmlElement* pRoot = xmlFile.findNodeByName( "xtemplate" );

		const XmlAttribute* pAttName = xmlFile.findAttributeByName( "name", pRoot );
		if ( pAttName == nullptr )
		{
			TIKI_TRACE_ERROR( "[convertermanager] name argument not found. can't add template: %s\n", fileName.cStr() );
			return;
		}

		TemplateDescription desc;
		desc.fullFileName	= path::getAbsolutePath( fileName );
		desc.name			= pAttName->content;

		// parse arguments
		parseParams( xmlFile, pRoot, desc.arguments );

		m_templates[ desc.name ] = desc;

		xmlFile.dispose();
	}

	bool ConverterManager::queueFile( const string& fileName )
	{
		string absoluteFileName	= path::getAbsolutePath( fileName );
		const string extension	= path::getExtension( absoluteFileName );
		if ( extension == ".xasset" )
		{
			const string fileTypeString = path::getFilenameWithoutExtension( fileName );

			FileDescription file;
			file.fullFileName	= absoluteFileName;
			file.fileType		= crcString( path::getExtension( fileTypeString ).subString( 1u ) );

			return internalQueueFile( file );
		}
		else
		{
			const string sql = formatString( "SELECT asset.* FROM dependencies as dep, assets as asset WHERE dep.type = '%u' AND dep.identifier = '%s' AND asset.id = dep.asset_id", ConversionResult::DependencyType_File, absoluteFileName.cStr() );

			List< FileDescription > filesToBuild;
			{
				AutoDispose< SqliteQuery > query;
				if ( query->create( m_dataBase, sql ) == false )
				{
					TIKI_TRACE_ERROR( "[convertermanager] SQL command failed. Error: %s\n", query->getLastError().cStr() );
					return false;
				}

				while ( query->nextRow() )
				{
					FileDescription& file = filesToBuild.add();
					file.fullFileName	= path::combine( query->getTextField( "path" ), query->getTextField( "filename" ) );
					file.fileType		= (crc32)query->getIntegerField( "type" );
				}
			}

			bool result = true;
			for (uint i = 0u; i < filesToBuild.getCount(); ++i)
			{
				result &= internalQueueFile( filesToBuild[ i ] );
			}

			return result;
		}
	}

	bool ConverterManager::startConversion( List< string >* pOutputFiles /* = nullptr */, Mutex* pConversionMutex /* = nullptr */ )
	{
		List< string > outputFiles;
		if ( pOutputFiles == nullptr )
		{
			pOutputFiles = &outputFiles;
		}

		for (uint i = 0u; i < m_tasks.getCount(); ++i)
		{
			ConversionTask& task = m_tasks[ i ];
			task.taskId = queueTask( taskConvertFile, &task );
		}

		m_taskSystem.waitForAllTasks();

		bool converionResult = true;
		for (uint i = 0u; i < m_tasks.getCount(); ++i)
		{
			const ConversionTask& task = m_tasks[ i ];
			converionResult &= finalizeFile( *pOutputFiles, task );
		}

		if ( converionResult )
		{
			m_resourceMap.writeToFile();
		}

		return converionResult;
	}

	void ConverterManager::registerConverter( const ConverterBase* pConverter )
	{
		m_converters.add( pConverter );
	}

	void ConverterManager::unregisterConverter( const ConverterBase* pConverter )
	{
		m_converters.remove( pConverter );
	}

	TaskId ConverterManager::queueTask( TaskFunc pFunc, void* pData, TaskId dependingTaskId /*= InvalidTaskId */ )
	{
		return m_taskSystem.queueTask( pFunc, pData, dependingTaskId );
	}

	void ConverterManager::waitForTask( TaskId taskId )
	{
		m_taskSystem.waitForTask( taskId );
	}

	void ConverterManager::traceCallback( cstring message, TraceLevel level ) const
	{
		string line = message;
		if ( !line.endsWith('\n') )
		{
			line += "\n";
		}

		m_loggingMutex.lock();
		m_loggingStream.write( line.cStr(), line.getLength() );
		m_loggingMutex.unlock();
	}

	void ConverterManager::parseParams( const TikiXml& xmlFile, const XmlElement* pRoot, std::map< string, string >& arguments ) const
	{
		const XmlElement* pParam = xmlFile.findFirstChild( "param", pRoot );
		while ( pParam )
		{
			const XmlAttribute* pAttKey		= xmlFile.findAttributeByName( "key", pParam );
			const XmlAttribute* pAttValue	= xmlFile.findAttributeByName( "value", pParam );

			if ( pAttKey == nullptr )
			{
				TIKI_TRACE_WARNING(
					"param failed: %s%s\n",
					( pAttKey == nullptr ? "no key-attribute " : string( "key: " ) + pAttKey->content ).cStr(),
					( pAttValue == nullptr ? "no value-attribute " : string( "value: " ) + pAttValue->content ).cStr()
				);
			}
			else if ( pAttValue == nullptr )
			{
				arguments[ pAttKey->content ] = pParam->content;
			}
			else
			{
				arguments[ pAttKey->content ] = pAttValue->content;
			}

			pParam = xmlFile.findNext( pParam );
		}
	}

	bool ConverterManager::checkChangesAndFillConversionParameters( ConversionParameters& params, const ConverterBase** ppConverter, const FileDescription& file )
	{
		const ConverterBase* pConverter = nullptr;
		for (size_t i = 0u; i < m_converters.getCount(); ++i)
		{
			if ( m_converters[ i ]->getInputType() == file.fileType )
			{
				pConverter = m_converters[ i ];
				break;
			}
		}

		if ( pConverter == nullptr )
		{
			TIKI_TRACE_ERROR( "No Converter found for file: '%s'.\n", file.fullFileName.cStr() );
			return false;
		}

		if (!file::exists( file.fullFileName ))
		{
			TIKI_TRACE_ERROR( "source file not found: %s\n", file.fullFileName.cStr() );
			return false;
		}


		params.targetPlatform	= getHostPlatform();
		params.targetApi		= getHostGraphicsApi();
		params.sourceFile		= file.fullFileName;
		params.typeCrc			= pConverter->getInputType();

		TikiXml xmlFile;
		xmlFile.create( file.fullFileName );

		// parse root node
		const XmlElement* pRoot = xmlFile.findNodeByName( "tikiasset" );
		if ( pRoot == nullptr )
		{
			TIKI_TRACE_ERROR( "no asset definition found.\n" );
			return false;
		}

		const XmlAttribute* pOutput = xmlFile.findAttributeByName( "output-name", pRoot );
		if ( pOutput == nullptr )
		{
			params.outputName = path::getFilenameWithoutExtension( path::getFilenameWithoutExtension( file.fullFileName ) );
			//TIKI_TRACE_DEBUG( "no output name given. use xasset filename: %s\n", params.outputName.cStr() );
		}
		else
		{
			params.outputName = pOutput->content;
		}

		// read inputs
		const string inputDir		= path::getDirectoryName( file.fullFileName );
		const XmlElement* pInput	= xmlFile.findNodeByName( "input" );
		while ( pInput != nullptr )
		{
			const XmlAttribute* pAttFile	= xmlFile.findAttributeByName( "file", pInput );
			const XmlAttribute* pAttType	= xmlFile.findAttributeByName( "type", pInput );

			if ( pAttFile == nullptr || pAttType == nullptr )
			{
				TIKI_TRACE_WARNING(
					"[ConverterManager] XASSET: Input-Tag has wrong attributes: %s%s\n",
					( pAttFile == nullptr ? "no file-attribute " : string( "file: " ) + pAttFile->content ).cStr(),
					( pAttType == nullptr ? "no type-attribute " : string( "type: " ) + pAttType->content ).cStr()
				);
			}
			else
			{
				ConversionParameters::InputFile input;
				input.fileName	= pAttFile->content;
				input.typeCrc	= crcString( pAttType->content );

				if ( !file::exists( input.fileName ) )
				{
					input.fileName = path::combine( inputDir, input.fileName );
				}

				if ( !file::exists( input.fileName ) )
				{
					TIKI_TRACE_WARNING( "input file not found. these will be ignored. path: %s\n", input.fileName.cStr() );
				}
				else
				{
					input.fileName = path::getAbsolutePath( input.fileName );
					params.inputFiles.add( input );
				}
			}

			pInput = xmlFile.findNext( "input", pInput );
		}

		if ( params.inputFiles.getCount() == 0u )
		{
			TIKI_TRACE_ERROR( "no inputs specified. asset can't be converted.\n" );
			xmlFile.dispose();
			return false;
		}

		// read params
		parseParams( xmlFile, pRoot, params.arguments.getMap() );

		const XmlAttribute* pTemplate = xmlFile.findAttributeByName( "template", pRoot );
		if ( pTemplate != nullptr )
		{
			TemplateMap::iterator temp = m_templates.find( pTemplate->content );
			if ( temp != m_templates.end() )
			{
				const TemplateDescription& desc = temp->second;

				for (std::map< string, string >::const_iterator it = desc.arguments.begin(); it != desc.arguments.end(); it++)
				{
					params.arguments.getMap()[ it->first ] = it->second;
				}
			}
		}

		xmlFile.dispose();

		params.assetId = 0u;
		if ( !writeConvertInput( params.assetId, params ) )
		{
			return false;
		}

		*ppConverter = pConverter;
		if ( !checkDependencies( params.assetId, pConverter ) )
		{
			// no build required
			return false;
		}

		return true;
	}

	bool ConverterManager::internalQueueFile( const FileDescription& file )
	{
		ConversionTask task;
		if ( !checkChangesAndFillConversionParameters( task.parameters, &task.pConverter, file ) )
		{
			return task.pConverter != nullptr;
		}

		task.result.addDependency( ConversionResult::DependencyType_Converter, "", "", task.pConverter->getConverterRevision() );
		task.result.addDependency( ConversionResult::DependencyType_File, task.parameters.sourceFile, "", 0u );
		for (uint i = 0u; i < task.parameters.inputFiles.getCount(); ++i)
		{
			const string& inputFileName = task.parameters.inputFiles[ i ].fileName;
			task.result.addDependency( ConversionResult::DependencyType_File, inputFileName, "", 0u );
		}

		 m_tasks.add(task);
		return true;
	}
	
	bool ConverterManager::finalizeFile( List< string >& outputFiles, const ConversionTask& task )
	{
		const List< string >& resultOutputFiles = task.result.getOutputFiles();
		for (uint i = 0u; i < resultOutputFiles.getCount(); ++i)
		{
			const string& resourceName = resultOutputFiles[ i ];

			outputFiles.add( resourceName );
			m_resourceMap.registerResource( path::getFilename( resourceName ) );
		}

		bool hasError = false;
		const List< ConversionResult::TraceInfo >& traceInfos = task.result.getTraceInfos();
		for (uint i = 0u; i < traceInfos.getCount(); ++i)
		{
			if ( traceInfos[ i ].level >= TraceLevel_Warning )
			{
				hasError = true;
				break;
			}
		}
		writeConvertResult( task.parameters.assetId, task.result, hasError );

		return !hasError;
	}

	uint ConverterManager::findAssetIdByName( const string& name )
	{
		if ( m_dataBase.isCreated() == false )
		{
			return TIKI_SIZE_T_MAX;
		}

		const string sql = formatString( "SELECT * FROM assets WHERE filename = '%s'", name.cStr() );

		AutoDispose< SqliteQuery > query;
		if ( query->create( m_dataBase, sql ) == false || query->nextRow() == false )
		{
			return TIKI_SIZE_T_MAX;
		}

		return (uint)query->getIntegerField( "id" );
	}

	bool ConverterManager::checkDependencies( uint assetId, const ConverterBase* pConverter )
	{
		if ( m_dataBase.isCreated() == false || m_rebuildForced || assetId == TIKI_SIZE_T_MAX )
		{
			return true;
		}

		// check error
		{
			const string sql = formatString( "SELECT * FROM assets WHERE id = '%u';", assetId );

			AutoDispose< SqliteQuery > query;
			if ( query->create( m_dataBase, sql ) == false )
			{
				TIKI_TRACE_ERROR( "[convertermanager] can't prepare sql command. error: %s\n", query->getLastError().cStr() );
				return true;
			}

			if ( query->nextRow() )
			{
				if ( query->getIntegerField( "has_error" ) )
				{
					return true;
				}
			}
			else
			{
				return true;
			}
		}


		// check dependencies
		bool buildNeeded = true;
		{
			const string sql = formatString( "SELECT * FROM dependencies WHERE asset_id='%u';", assetId );

			AutoDispose< SqliteQuery > query;
			if ( query->create( m_dataBase, sql ) == false )
			{
				TIKI_TRACE_ERROR( "[convertermanager] can't prepare sql command. error: %s\n", query->getLastError().cStr() );
				return true;
			}

			while ( query->nextRow() )
			{
				const ConversionResult::DependencyType type = (ConversionResult::DependencyType)query->getIntegerField( "type" );

				const string identifier	= query->getTextField( "identifier" );

				const int valueInt		= query->getIntegerField( "value_int" );
				const string valueText	= query->getTextField( "value_text" );

				switch ( type )
				{
				case ConversionResult::DependencyType_Converter:
					{
						if ( (uint)valueInt != pConverter->getConverterRevision() || pConverter->getConverterRevision() == TIKI_SIZE_T_MAX )
						{
							return true;
						}
					}
					break;

				case ConversionResult::DependencyType_File:
					{
						const crc32 fileChangeCrc = file::getLastChangeCrc( identifier );
						if ( fileChangeCrc != (crc32)valueInt )
						{
							return true;
						}
					}
					break;

				case ConversionResult::DependencyType_Type:
					{
						return true;
					}
					break;

				}

				buildNeeded = false;
			}
		}

		return buildNeeded;
	}

	bool ConverterManager::writeConvertInput( uint& assetId, const ConversionParameters& parametes )
	{
		const string filePath = path::getDirectoryName( parametes.sourceFile );
		const string fileName = path::getFilename( parametes.sourceFile );

		assetId = findAssetIdByName( fileName );
		if ( assetId == TIKI_SIZE_T_MAX )
		{
			const bool sqlResult = m_dataBase.executeCommand(
				formatString(
					"INSERT INTO assets (filename,path,type,has_error) VALUES ('%s','%s','%u','0');",
					fileName.cStr(),
					filePath.cStr(),
					parametes.typeCrc
				)
			);
			if ( sqlResult == false )
			{
				TIKI_TRACE_ERROR( "[convertermanager] SQL command failed. Error: %s\n", m_dataBase.getLastError().cStr() );
				return false;
			}
			assetId = m_dataBase.getLastInsertId();
		}
		else
		{
			const bool sqlResult = m_dataBase.executeCommand(
				formatString( "DELETE FROM input_files WHERE asset_id = '%u';", assetId )
			);
			if ( sqlResult == false )
			{
				TIKI_TRACE_ERROR( "[convertermanager] SQL command failed. Error: %s\n", m_dataBase.getLastError().cStr() );
				return false;
			}
		}

		for (uint i = 0u; i < parametes.inputFiles.getCount(); ++i)
		{
			const ConversionParameters::InputFile& inputFile = parametes.inputFiles[ i ];
			const string inputFileName = path::getAbsolutePath( inputFile.fileName );

			const bool sqlResult = m_dataBase.executeCommand(
				formatString(
					"INSERT INTO input_files (asset_id,filename,type) VALUES ('%u','%s','%u');",
					assetId,
					inputFileName.cStr(),
					inputFile.typeCrc
				)
			);
			if ( sqlResult == false )
			{
				TIKI_TRACE_ERROR( "[convertermanager] SQL command failed. Error: %s\n", m_dataBase.getLastError().cStr() );
				return false;
			}
		}

		return true;
	}

	bool ConverterManager::writeConvertResult( uint assetId, const ConversionResult& result, bool hasError )
	{
		// delete old stuff
		{
			const string deleteCommands[] =
			{
				formatString( "DELETE FROM dependencies WHERE asset_id = '%u';", assetId ),
				formatString( "DELETE FROM output_files WHERE asset_id = '%u';", assetId ),
				formatString( "DELETE FROM traces WHERE asset_id = '%u';", assetId )
			};

			for (uint i = 0u; i < TIKI_COUNT( deleteCommands ); ++i)
			{
				const bool sqlResult = m_dataBase.executeCommand( deleteCommands[ i ] );
				if ( sqlResult == false )
				{
					TIKI_TRACE_ERROR( "[convertermanager] SQL command failed. Error: %s\n", m_dataBase.getLastError().cStr() );
					return false;
				}
			}
		}

		// dependencies
		{
			const List< ConversionResult::Dependency >& dependencies = result.getDependencies();
			for (uint i = 0u; i < dependencies.getCount(); ++i)
			{
				const ConversionResult::Dependency& dependency = dependencies[ i ];

				const bool sqlResult = m_dataBase.executeCommand(
					formatString(
						"INSERT INTO dependencies (asset_id,type,identifier,value_int,value_text) VALUES ('%u','%u','%s','%u','%s');",
						assetId,
						dependency.type,
						dependency.identifier.cStr(),
						dependency.valueInt,
						dependency.valueText.cStr()
					)
				);

				if ( sqlResult == false )
				{
					TIKI_TRACE_ERROR( "[convertermanager] SQL command failed. Error: %s\n", m_dataBase.getLastError().cStr() );
					return false;
				}
			}
		}

		// traces
		{
			const List< ConversionResult::TraceInfo >& traceInfos = result.getTraceInfos();
			for (uint i = 0u; i < traceInfos.getCount(); ++i)
			{
				const ConversionResult::TraceInfo& traceInfo = traceInfos[ i ];

				const bool sqlResult = m_dataBase.executeCommand(
					formatString(
						"INSERT INTO traces (asset_id,level,message) VALUES ('%u','%u','%s');",
						assetId,
						traceInfo.level,
						escapeString( traceInfo.message ).cStr()
					)
				);

				if ( sqlResult == false )
				{
					TIKI_TRACE_ERROR( "[convertermanager] SQL command failed. Error: %s\n", m_dataBase.getLastError().cStr() );
					return false;
				}
			}
		}

		// output files
		{
			const List< string >& outputFiles = result.getOutputFiles();
			for (uint i = 0u; i < outputFiles.getCount(); ++i)
			{
				const string& outputFile = outputFiles[ i ];

				const bool sqlResult = m_dataBase.executeCommand(
					formatString(
						"INSERT INTO output_files (asset_id,filename) VALUES ('%u','%s');",
						assetId,
						outputFile.cStr()
					)
				);

				if ( sqlResult == false )
				{
					TIKI_TRACE_ERROR( "[convertermanager] SQL command failed. Error: %s\n", m_dataBase.getLastError().cStr() );
					return false;
				}
			}
		}

		// update asset
		{
			const int errorValue = hasError;
			const bool sqlResult = m_dataBase.executeCommand(
				formatString( "UPDATE assets SET has_error = '%u' WHERE id = '%u';", errorValue, assetId )
			);

			if ( sqlResult == false )
			{
				TIKI_TRACE_ERROR( "[convertermanager] SQL command failed. Error: %s\n", m_dataBase.getLastError().cStr() );
				return false;
			}
		}

		return true;
	}

	void ConverterManager::taskConvertFile( const TaskContext& context )
	{
		TIKI_ASSERT( context.pTaskData != nullptr );
		ConversionTask& task = *static_cast< ConversionTask* >( context.pTaskData );

		TIKI_TRACE_INFO( "Building asset: %s\n", path::getFilename( task.parameters.sourceFile ).cStr() );
		task.pConverter->convert( task.result, task.parameters );
	}
}
