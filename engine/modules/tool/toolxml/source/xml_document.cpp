#include "tiki/toolxml/xml_document.hpp"

namespace tiki
{
	bool XmlDocument::loadFromFile( const char* pFilename )
	{
		const tinyxml2::XMLError error = m_document.LoadFile( pFilename );
		if( error != tinyxml2::XML_SUCCESS )
		{
			TIKI_TRACE_ERROR( "[toolxml] Parse from file %s returned an error. %s\n", pFilename, m_document.GetErrorStr1() );
			return false;
		}

		return true;
	}

	bool XmlDocument::loadFromString( const char* pString )
	{
		const tinyxml2::XMLError error = m_document.Parse( pString );
		if( error != tinyxml2::XML_SUCCESS )
		{
			TIKI_TRACE_ERROR( "[toolxml] XML parser returned an error. %s\n", m_document.GetErrorStr1() );
			return false;
		}

		return true;
	}

	bool XmlDocument::saveToString( string& target )
	{
		tinyxml2::XMLPrinter printer;
		m_document.Print( &printer );

		target = printer.CStr();
	}

	bool XmlDocument::saveToFile( const char* pFilename )
	{
		const tinyxml2::XMLError error = m_document.SaveFile( pFilename );
		if( error != tinyxml2::XML_SUCCESS )
		{
			TIKI_TRACE_ERROR( "[toolxml] Save to %s failed with error: %s\n", pFilename, m_document.GetErrorStr1() );
			return false;
		}

		return true;
	}

}
