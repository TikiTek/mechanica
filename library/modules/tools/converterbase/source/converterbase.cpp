
#include "tiki/converterbase/converterbase.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/file.hpp"
#include "tiki/base/iopath.hpp"
#include "tiki/converterbase/conversionparameters.hpp"
#include "tiki/converterbase/convertermanager.hpp"
#include "tiki/converterbase/resourcewriter.hpp"
#include "tiki/toolbase/tikixml.hpp"

namespace tiki
{
	ConverterBase::ConverterBase()
	{
		m_pManager = nullptr;
	}

	ConverterBase::~ConverterBase()
	{
		TIKI_ASSERT( m_pManager == nullptr );
	}

	void ConverterBase::create( ConverterManager* pManager )
	{
		TIKI_ASSERT( m_pManager == nullptr );
		m_pManager = pManager;
		m_pManager->registerConverter( this );

		initializeConverter();
	}

	void ConverterBase::dispose()
	{
		TIKI_ASSERT( m_pManager );

		disposeConverter();

		m_pManager->unregisterConverter( this );
		m_pManager = nullptr;
	}

	void ConverterBase::convert( const ConversionParameters& params ) const
	{
		TIKI_ASSERT( m_pManager );
		
		if ( !startConversionJob( params ) )
		{
			TIKI_TRACE_ERROR( "converter returns an error.\n" );
		}
	}

	void ConverterBase::openResourceWriter( ResourceWriter* pWriter, fourcc cc, const string& fileName, const string& extension ) const
	{
		string realName = path::combine( m_pManager->getOutputPath(), fileName + "." + extension );
		pWriter->create( cc, realName );
	}

	void ConverterBase::closeResourceWriter( ResourceWriter* pWriter ) const
	{
		pWriter->dispose();
	}

}
