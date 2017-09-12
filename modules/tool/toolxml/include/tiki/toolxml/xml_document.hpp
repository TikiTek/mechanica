#pragma once
#ifndef TIKI_XML_DOCUMENT_HPP_INCLUDED
#define TIKI_XML_DOCUMENT_HPP_INCLUDED

#include "tiki/toolxml/xml_node.hpp"

#include "tiki/base/dynamic_string.hpp"

#include <tinyxml2.h>

namespace tiki
{
	class XmlDocument : public XmlNode
	{
	public:

								XmlDocument();
								~XmlDocument();

		bool					loadFromFile( const char* pFilename );
		bool					loadFromString( const char* pString );

		void					saveToString( string& target );
		bool					saveToFile( const char* pFilename );

		XmlElement*				getRoot();
		const XmlElement*		getRoot() const;

		XmlElement*				createElement( const char* pName );
		void					destroyElement( const XmlElement* pElement );

		void					appendNode( XmlElement* pElement );
		void					removeNode( XmlElement* pElement );

	private:

		tinyxml2::XMLDocument	m_document;
	};
}

#endif // TIKI_XML_DOCUMENT_HPP_INCLUDED
