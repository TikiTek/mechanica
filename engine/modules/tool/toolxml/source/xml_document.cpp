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

	XmlNode* XmlDocument::getRoot()
	{
		return (XmlNode*)m_document.RootElement();
	}

	const XmlNode* XmlDocument::getRoot() const
	{
		return (const XmlNode*)m_document.RootElement();
	}

	XmlNode* XmlDocument::createNode( const char* pName )
	{
		return (XmlNode*)m_document.NewElement( pName );
	}

	void XmlDocument::destroyNode( const XmlNode* pNode )
	{
		m_document.DeleteNode( (tinyxml2::XMLNode*)pNode );
	}

	void XmlDocument::appendNode( XmlNode* pNode )
	{
		m_document.InsertEndChild( (tinyxml2::XMLNode*)pNode );
	}

	void XmlDocument::removeNode( XmlNode* pNode )
	{
		m_document.DeleteChild( (tinyxml2::XMLNode*)pNode );
	}
}
