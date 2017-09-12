#pragma once
#ifndef TIKI_XML_ELEMENT_HPP_INCLUDED
#define TIKI_XML_ELEMENT_HPP_INCLUDED

#include "tiki/toolxml/xml_node.hpp"

namespace tinyxml2
{
	class XMLElement;
}

namespace tiki
{
	class XmlAttribute;

	class XmlElement : public XmlNode
	{
	public:

		const char*					getName() const;
		bool						isName( const char* pName ) const;

		const XmlAttribute*			getFirstAttribute() const;
		const XmlAttribute*			findAttribute( const char* pName ) const;
		void						setAttribute( const char* pName, const char* pValue );

	protected:

									XmlElement();
									~XmlElement();

		tinyxml2::XMLElement*		getNativeElement();
		const tinyxml2::XMLElement*	getNativeElement() const;
	};
}

#endif // TIKI_XML_NODE_HPP_INCLUDED
