#pragma once

#include "tiki/base/types.hpp"

namespace tinyxml2
{
	class XMLNode;
}

namespace tiki
{
	class XmlDocument;
	class XmlElement;

	class XmlNode
	{
	public:

		XmlDocument&				getDocument();
		const XmlDocument&			getDocument() const;

		const char*					getValue() const;
		void						setValue( const char* pValue );

		XmlNode*					getParent();
		const XmlNode*				getParent() const;

		XmlElement*					getPreviousSibling();
		const XmlElement*			getPreviousSibling() const;
		XmlElement*					getNextSibling();
		const XmlElement*			getNextSibling() const;
		XmlElement*					findNextSibling( const char* pName );
		const XmlElement*			findNextSibling( const char* pName ) const;
		XmlElement*					findNextSiblingWithSameName();
		const XmlElement*			findNextSiblingWithSameName() const;

		XmlElement*					getFirstChild();
		const XmlElement*			getFirstChild() const;
		XmlElement*					getLastChild();
		const XmlElement*			getLastChild() const;
		XmlElement*					findFirstChild( const char* pName );
		const XmlElement*			findFirstChild( const char* pName ) const;

		void						appendChild( XmlElement* pElement );
		void						removeChild( XmlElement* pElement );

		uint						getLineNumber() const;

	protected:

									XmlNode();
									~XmlNode();

		tinyxml2::XMLNode*			getNativeNode();
		const tinyxml2::XMLNode*	getNativeNode() const;
	};
}
