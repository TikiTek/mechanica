#pragma once
#ifndef TIKI_XML_DOCUMENT_HPP_INCLUDED
#define TIKI_XML_DOCUMENT_HPP_INCLUDED

#include "tiki/base/dynamic_string.hpp"

#include <tinyxml2.h>

namespace tiki
{
	class XmlNode;

	class XmlDocument
	{
	public:

		bool					loadFromFile( const char* pFilename );
		bool					loadFromString( const char* pString );

		bool					saveToString( string& target );
		bool					saveToFile( const char* pFilename );

		XmlNode*				getRoot();
		const XmlNode*			getRoot() const;

		XmlNode*				createNode( const char* pName );
		void					destroyNode( const XmlNode* pNode );

		void					appendNode( XmlNode* pNode );
		void					removeNode( XmlNode* pNode );

	private:

		tinyxml2::XMLDocument	m_document;
	};
}

#endif // TIKI_XML_DOCUMENT_HPP_INCLUDED
