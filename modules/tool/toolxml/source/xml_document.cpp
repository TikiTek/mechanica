#include "tiki/toolxml/xml_document.hpp"

namespace tiki
{
	XmlDocument::XmlDocument()
	{
	}

	XmlDocument::~XmlDocument()
	{
	}

	bool XmlDocument::loadFromFile( const char* pFilename )
	{
		TIKI_COMPILETIME_ASSERT( TIKI_OFFSETOF( XmlDocument, m_document ) == 0u );

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

	void XmlDocument::saveToString( string& target )
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

	XmlElement* XmlDocument::getRoot()
	{
		return ( XmlElement* )m_document.RootElement();
	}

	const XmlElement* XmlDocument::getRoot() const
	{
		return ( const XmlElement* )m_document.RootElement();
	}

	XmlElement* XmlDocument::createElement( const char* pName )
	{
		return ( XmlElement* )m_document.NewElement( pName );
	}

	void XmlDocument::destroyElement( const XmlElement* pElement )
	{
		m_document.DeleteNode( (tinyxml2::XMLNode*)pElement );
	}

	void XmlDocument::appendNode( XmlElement* pElement )
	{
		m_document.InsertEndChild( (tinyxml2::XMLNode*)pElement );
	}

	void XmlDocument::removeNode( XmlElement* pElement )
	{
		m_document.DeleteChild( (tinyxml2::XMLNode*)pElement );
	}
}
