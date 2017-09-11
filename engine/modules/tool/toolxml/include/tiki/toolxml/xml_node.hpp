#pragma once
#ifndef TIKI_XML_NODE_HPP_INCLUDED
#define TIKI_XML_NODE_HPP_INCLUDED

namespace tinyxml2
{
	class XMLNode;
}

namespace tiki
{
	class XmlAttribute;

	class XmlNode
	{
	public:

		XmlDocument*		getDocument();
		const XmlDocument*	getDocument() const;

		const char*			getName() const;
		const char*			getValue() const;
		void				setValue( const char* pValue );

		XmlAttribute*		getFirstAttribute();
		const XmlAttribute*	getFirstAttribute() const;
		XmlAttribute*		getLastAttribute();
		const XmlAttribute*	getLastAttribute() const;
		XmlAttribute*		findAttribute( const char* pName );
		const XmlAttribute*	findAttribute( const char* pName ) const;

		void				appendAttribute( XmlAttribute* pAttribute );
		void				removeAttribute( XmlAttribute* pAttribute );
		void				setAttribute( const char* pName, const char* pValue );

		XmlNode*			getParent();
		const XmlNode*		getParent() const;

		XmlNode*			getPreviousSibling();
		const XmlNode*		getPreviousSibling() const;
		XmlNode*			getNextSibling();
		const XmlNode*		getNextSibling() const;
		XmlNode*			findNextSibling( const char* pName );
		const XmlNode*		findNextSibling( const char* pName ) const;

		XmlNode*			getFirstChild();
		const XmlNode*		getFirstChild() const;
		XmlNode*			getLastChild();
		const XmlNode*		getLastChild() const;
		XmlNode*			findFirstChild( const char* pName );
		const XmlNode*		findFirstChild( const char* pName ) const;

		void				appendChild( XmlNode* pNode );
		void				removeChild( XmlNode* pNode );

	private:

							XmlNode();
							~XmlNode();

		tinyxml2::XMLNode*	getNativeNode();
	};
}

#endif // TIKI_XML_NODE_HPP_INCLUDED
