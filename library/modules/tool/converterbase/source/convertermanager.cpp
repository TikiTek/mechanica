
#include "tiki/converterbase/convertermanager.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/base/file.hpp"
#include "tiki/base/iopath.hpp"
#include "tiki/base/platform.hpp"
#include "tiki/converterbase/conversionparameters.hpp"
#include "tiki/converterbase/converterbase.hpp"
#include "tiki/toolbase/tikixml.hpp"

#include "sqlite/sqlite3.h"

namespace tiki
{	
	ConverterManager* ConverterManager::s_pInstance = nullptr;

	void globalTraceCallback( cstring message, debug::TraceLevel level )
	{
		ConverterManager::s_pInstance->traceCallback( message, level );
	}

	ConverterManager::~ConverterManager()
	{
		TIKI_ASSERT( m_converters.getCount() == 0u );
	}

	void ConverterManager::create( const ConverterManagerParameter& parameters )
	{
		m_outputPath	= parameters.outputPath;
		m_returnValue	= 0;
		m_rebuildForced	= parameters.forceRebuild;
		m_resourceMap.create( path::combine( m_outputPath, "resourcenamemap.rnm" ) );

		bool newDatabase = !file::exists( "build.sqlite" );
		if ( sqlite3_open( "build.sqlite", &m_pDataBase ) != SQLITE_OK )
		{
			m_pDataBase = nullptr;
			TIKI_TRACE_ERROR( "[convertermanager] database intialization failed.\n" );
		}

		if ( newDatabase )
		{
			char* errorMsg;
			cstring sql = "CREATE TABLE \"files\" (\"id\" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE , \"filename\" TEXT NOT NULL , \"time_id\" INTEGER NOT NULL , \"content_id\" INTEGER NOT NULL , \"converter_revision\" INTEGER NOT NULL );";

			if ( sqlite3_exec( m_pDataBase, sql, nullptr, nullptr, &errorMsg ) != SQLITE_OK )
			{
				TIKI_TRACE_ERROR( "[convertermanager] can't create database. Error: %s\n", errorMsg );
			}
		}

		m_loggingMutex.create();
		m_loggingStream.open( "converter.log", DataAccessMode_WriteAppend );

		s_pInstance = this;
		debug::setTraceCallback( globalTraceCallback );

		TIKI_TRACE( "ConverterManager: started\n" );
	}

	void ConverterManager::dispose()
	{
		TIKI_TRACE( "ConverterManager: shutdown\n" );

		debug::setTraceCallback( nullptr );

		m_resourceMap.dispose();

		m_loggingStream.close();
		m_loggingMutex.dispose();

		if ( m_pDataBase != nullptr )
		{
			sqlite3_close( m_pDataBase );
			m_pDataBase = nullptr;
		}
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
		desc.fullFileName	= fileName;
		desc.name			= pAttName->content;
		
		// parse arguments
		parseParams( xmlFile, pRoot, desc.arguments );

		m_templates[ desc.name ] = desc;

		xmlFile.dispose();
	}

	void ConverterManager::queueFile( const string& fileName )
	{
		string nameData = path::getFilenameWithoutExtension( fileName );

		FileDescription& file = m_files.add();

		file.fullFileName	= fileName;
		file.fileType		= crcString( path::getExtension( nameData ).substring( 1u ) );
		file.lastChange		= 0u;
	}
	
	int ConverterManager::startConversion()
	{
		crc32 currentType				= 0u;
		const ConverterBase* pConverter	= nullptr;
		
		for (size_t i = 0u; i < m_files.getCount(); ++i)
		{
			const FileDescription& file = m_files[ i ];

			//TIKI_TRACE( "Building asset: %s\n", file.fullFileName.cStr() );

			if ( currentType != file.fileType )
			{
				pConverter	= nullptr;
				currentType	= InvalidCrc32;

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
					continue;
				}

				currentType	= file.fileType;
			}
			
			convertFile( pConverter, file.fullFileName );
		}

		return m_returnValue;
	}

	void ConverterManager::registerConverter( const ConverterBase* pConverter )
	{
		m_converters.add( pConverter );
	}

	void ConverterManager::unregisterConverter( const ConverterBase* pConverter )
	{
		m_converters.remove( pConverter );
	}

	void ConverterManager::traceCallback( cstring message, debug::TraceLevel level ) const
	{
		string line = message;

		if ( !line.endsWith('\n') )
		{
			line += "\n";
		}

		m_loggingMutex.lock();
		m_loggingStream.write( line.cStr(), line.getLength() );

		if ( level >= debug::TraceLevel_Warning )
		{
			m_returnValue = 1;
		}

		m_loggingMutex.unlock();			
	}

	bool ConverterManager::checkBuildNeeded( const string& fileName, const size_t converterRevision )
	{
		if ( m_pDataBase == nullptr )
		{
			return true;
		}

		const crc32 fileHash = 0u;
		const crc32 fileTime = file::getLastChangeCrc( fileName );

		if ( m_rebuildForced )
		{
			char* pErrorMsg;
			const string sql2 = formatString( "UPDATE files SET time_id=%i, content_id=%i, converter_revision=%i WHERE filename='%s'", fileTime, fileHash, converterRevision, fileName.cStr() );

			if ( sqlite3_exec( m_pDataBase, sql2.cStr(), nullptr, nullptr, &pErrorMsg ) != SQLITE_OK )
			{
				TIKI_TRACE_ERROR( "[convertermanager] can't update file in database. error: %s\n", pErrorMsg );
			}

			return true;
		}

		const string sql = formatString( "SELECT * FROM files WHERE filename='%s';", fileName.cStr() );

		cstring pTail;
		sqlite3_stmt* pState;
		
		if ( sqlite3_prepare( m_pDataBase, sql.cStr(), sql.getLength(), &pState, &pTail ) != SQLITE_OK )
		{
			TIKI_TRACE_ERROR( "[convertermanager] can't prepare sql command. error: %s\n", pTail );
			return true;
		}

		bool returnValue = true;
		
		//Array< uint8 > fileContent;
		//file::readAllBytes( fileName, fileContent );		
		//const crc32 fileHash = crcBytes( fileContent.getData(), fileContent.getCount() );
		//fileContent.dispose();

		if ( sqlite3_step( pState ) == SQLITE_ROW )
		{
			crc32 sqlTime	= (crc32)sqlite3_column_int( pState, 2 );
			crc32 sqlHash	= (crc32)sqlite3_column_int( pState, 3 );
			crc32 sqlRev	= (crc32)sqlite3_column_int( pState, 4 );

			returnValue = !( (fileTime == sqlTime) && (fileHash == sqlHash) && (converterRevision == sqlRev) );

			if ( returnValue )
			{
				char* pErrorMsg;
				const string sql2 = formatString( "UPDATE files SET time_id=%i, content_id=%i, converter_revision=%i WHERE filename='%s'", fileTime, fileHash, converterRevision, fileName.cStr() );

				if ( sqlite3_exec( m_pDataBase, sql2.cStr(), nullptr, nullptr, &pErrorMsg ) != SQLITE_OK )
				{
					TIKI_TRACE_ERROR( "[convertermanager] can't update file in database. error: %s\n", pErrorMsg );
				}
			}
		}
		else
		{
			char* pErrorMsg;
			const string sql2 = formatString( "INSERT INTO files (filename,time_id,content_id,converter_revision) VALUES ('%s',%i,%i,%i)", fileName.cStr(), fileTime, fileHash, converterRevision );
			
			if ( sqlite3_exec( m_pDataBase, sql2.cStr(), nullptr, nullptr, &pErrorMsg ) != SQLITE_OK )
			{
				TIKI_TRACE_ERROR( "[convertermanager] can't intsert file into database. error: %s\n", pErrorMsg );
			}
		}

		sqlite3_finalize( pState );

		return returnValue;
	}

	void ConverterManager::parseParams( const TikiXml& xmlFile, const XmlElement* pRoot, std::map< string, string >& arguments ) const
	{
		const XmlElement* pParam = xmlFile.findFirstChild( "param", pRoot );
		while ( pParam )
		{
			const XmlAttribute* pAttKey		= xmlFile.findAttributeByName( "key", pParam );
			const XmlAttribute* pAttValue	= xmlFile.findAttributeByName( "value", pParam );

			if ( pAttKey == nullptr || pAttValue == nullptr )
			{
				TIKI_TRACE_WARNING(
					"param failed: %s%s\n",
					( pAttKey == nullptr ? "no key-attribute " : string( "key: " ) + pAttKey->content ).cStr(),
					( pAttValue == nullptr ? "no value-attribute " : string( "value: " ) + pAttValue->content ).cStr()
				);
			}
			else
			{
				arguments[ pAttKey->content ] = pAttValue->content;
			}

			pParam = xmlFile.findNext( pParam );
		}
	}

	void ConverterManager::convertFile( const ConverterBase* pConverter, const string& sourceFile )
	{
		if (!file::exists( sourceFile ))
		{
			TIKI_TRACE_ERROR( "source file not found: %s\n", sourceFile.cStr() );
			return;
		}

		bool needBuild = checkBuildNeeded( sourceFile, pConverter->getConverterRevision() );

		ConversionParameters params;
		params.targetPlatform	= PlatformType_Win;
		params.sourceFile		= sourceFile;

		TikiXml xmlFile;
		xmlFile.create( sourceFile );

		// parse root node
		const XmlElement* pRoot = xmlFile.findNodeByName( "tikiasset" );
		if ( pRoot == nullptr )
		{
			TIKI_TRACE_ERROR( "no asset definition found.\n" );
			return;
		}

		const XmlAttribute* pOutput = xmlFile.findAttributeByName( "output-name", pRoot );
		if ( pOutput == nullptr )
		{
			params.outputName = path::getFilenameWithoutExtension( path::getFilenameWithoutExtension( sourceFile ) );
			TIKI_TRACE_DEBUG( "no output name given. use xasset filename: %s\n", params.outputName.cStr() );
		}
		else
		{
			params.outputName = pOutput->content;
		}

		// read inputs
		const string inputDir				= path::getDirectoryName( sourceFile );
		const XmlElement* pInput			= xmlFile.findNodeByName( "input" );
		while ( pInput )
		{
			const XmlAttribute* pAttFile	= xmlFile.findAttributeByName( "file", pInput );
			const XmlAttribute* pAttType	= xmlFile.findAttributeByName( "type", pInput );

			if ( pAttFile == nullptr || pAttType == nullptr )
			{
				TIKI_TRACE_WARNING(
					"input failed: %s%s\n",
					( pAttFile == nullptr ? "no file-attribute " : string( "file: " ) + pAttFile->content ).cStr(),
					( pAttType == nullptr ? "no type-attribute " : string( "type: " ) + pAttType->content ).cStr()
				);
			}
			else
			{
				ConversionInputFile input;
				input.typeString			= pAttType->content;
				input.inputType				= crcString( input.typeString );

				input.fileName				= pAttFile->content;
				if ( !file::exists( input.fileName ) )
				{
					input.fileName			= path::combine( inputDir, input.fileName );
				}

				if ( !file::exists( input.fileName ) )
				{
					TIKI_TRACE_WARNING( "input file not found. these will be ignored. path: %s\n", input.fileName.cStr() );
				}
				else
				{
					params.inputFiles.add( input );
				}

				needBuild |= checkBuildNeeded( input.fileName, pConverter->getConverterRevision() );
			}

			pInput = xmlFile.findNext( "input", pInput );
		}

		if ( params.inputFiles.getCount() == 0u )
		{
			TIKI_TRACE_ERROR( "no inputs specified. asset can't be converted.\n" );
			xmlFile.dispose();
			return;
		}

		// read params
		parseParams( xmlFile, pRoot, params.arguments.getMap() );

		const XmlAttribute* pTemplate = xmlFile.findAttributeByName( "template", pRoot );
		if ( pTemplate != nullptr )
		{
			auto temp = m_templates.find( pTemplate->content );

			if ( temp != m_templates.end() )
			{
				const TemplateDescription& desc = temp->second;

				for (auto it = desc.arguments.begin(); it != desc.arguments.end(); it++)
				{
					params.arguments.getMap()[ it->first ] = it->second;
				}
			}
		}

		xmlFile.dispose();	

		if ( needBuild == false )
		{
			//TIKI_TRACE( "File skipped.\n" );
			return;
		}
		else
		{
			TIKI_TRACE( "Building asset: %s\n", sourceFile.cStr() );
		}

		pConverter->convert( params );
	}
}