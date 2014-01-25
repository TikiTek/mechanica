#pragma once
#ifndef TIKI_CONVERTERMANAGER_HPP
#define TIKI_CONVERTERMANAGER_HPP

#include "tiki/base/staticarray.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"
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
		int						startConversion();
		bool					startConvertFile( const string& fileName );

		// converter
		void					registerConverter( const ConverterBase* pConverter );
		void					unregisterConverter( const ConverterBase* pConverter );

		// misc
		const string&			getOutputPath() const { return m_outputPath; }
		ConverterResourceMap&	getResourceMap() { return m_resourceMap; }

	private:

		typedef List< const ConverterBase* > ConverterList;

		struct FileDescription
		{
			string		fullFileName;
			crc32		fileType;

			uint32		lastChange;
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

		mutable Mutex				m_loggingMutex;
		mutable FileStream			m_loggingStream;
		mutable int					m_returnValue;

		FileList					m_files;
		TemplateMap					m_templates;
		ConverterList				m_converters;

		ConverterResourceMap		m_resourceMap;

		void						traceCallback( cstring message, TraceLevel level ) const;
		bool						checkBuildNeeded( const string& fileName, const size_t converterRevision );
		void						parseParams( const TikiXml& xmlFile, const _XmlElement* pRoot, std::map< string, string >& arguments ) const;

		bool						convertFile( const ConverterBase* pConverter, const string& fileName );

		uint						findAssetIdByName( const string& name );
		bool						checkDependencies( uint assetId, uint converterRevision );
		bool						writeConvertInput( uint assetId, const ConversionParameters& parametes );
		bool						writeConvertResult( uint assetId, const ConversionResult& result );

	};
}

#endif // TIKI_CONVERTERMANAGER_HPP
