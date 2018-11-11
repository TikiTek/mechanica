#pragma once

#include "tiki/tool_xml/xml_node.hpp"

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
