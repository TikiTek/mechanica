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

#include <map>

struct _XmlElement;

namespace tiki
{
	class ConverterBase;
	class TikiXml;
	struct ConversionParameters;
	struct ConversionResult;

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
		int						startConversion( Mutex* pConversionMutex = nullptr );
		bool					startConvertFile( const string& fileName, List< string >& outputFiles, Mutex* pConversionMutex = nullptr );

		// converter
		void					registerConverter( const ConverterBase* pConverter );
		void					unregisterConverter( const ConverterBase* pConverter );

		// resource map
		void					registerResource( const string& resourceName );
		void					writeResourceMap();
		void					addDependency( ConversionResult::DependencyType type, const string& identifier, const string& valueText, int valueInt );

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

		string						m_outputPath;

		SqliteDatabase				m_dataBase;
		bool						m_rebuildForced;
		bool						m_isNewDatabase;

		mutable Mutex				m_loggingMutex;
		mutable FileStream			m_loggingStream;
		mutable int					m_returnValue;

		FileList					m_files;
		TemplateMap					m_templates;
		ConverterList				m_converters;

		ConverterResourceMap		m_resourceMap;

		void						traceCallback( cstring message, TraceLevel level ) const;
		void						parseParams( const TikiXml& xmlFile, const _XmlElement* pRoot, std::map< string, string >& arguments ) const;

		bool						checkChangesAndFillConversionParameters( ConversionParameters& params, const ConverterBase** ppConverter, const FileDescription& file );
		bool						convertFile( const FileDescription& file, List< string >& outputFiles, Mutex* pConversionMutex );

		uint						findAssetIdByName( const string& name );
		bool						writeConvertInput( uint& assetId, const ConversionParameters& parametes );
		bool						checkDependencies( uint assetId, const ConverterBase* pConverter );
		bool						writeConvertResult( uint assetId, const ConversionResult& result, bool hasError );

	};
}

#endif // TIKI_CONVERTERMANAGER_HPP
