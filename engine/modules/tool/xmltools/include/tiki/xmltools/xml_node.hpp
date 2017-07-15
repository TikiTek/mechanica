#pragma once
#ifndef TIKI_XML_NODE_HPP_INCLUDED
#define TIKI_XML_NODE_HPP_INCLUDED

namespace tiki
{
	class XmlNode
	{
	public:

		const char*			getValue() const;

		XmlNode*			getParent();
		const XmlNode*		getParent() const;

		XmlNode*			getNextSibling();
		const XmlNode*		getNextSibling() const;

		XmlNode*			getFirstChild();
		const XmlNode*		getFirstChild() const;
		XmlNode*			getLastChild();
		const XmlNode*		getLastChild() const;

		void				appendChild( XmlNode* pNode );
		void				removeChild( XmlNode* pNode );

	private:

							XmlNode();
							~XmlNode();

		tinyxml2::XMLNode*	getNativeNode();
	};
}

#endif // TIKI_XML_NODE_HPP_INCLUDED
