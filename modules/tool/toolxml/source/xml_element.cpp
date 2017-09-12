#include "tiki/toolxml/xml_element.hpp"

namespace tiki
{
	const char* XmlElement::getName() const
	{
		return getNativeElement()->Name();
	}

	bool XmlElement::isName( const char* pName ) const
	{
		return isStringEquals( getNativeElement()->Name(), pName );
	}

	const XmlAttribute* XmlElement::getFirstAttribute() const
	{
		return ( const XmlAttribute* )getNativeElement()->FirstAttribute();
	}

	const XmlAttribute* XmlElement::findAttribute( const char* pName ) const
	{
		return ( const XmlAttribute* )getNativeElement()->FindAttribute( pName );
	}

	void XmlElement::setAttribute( const char* pName, const char* pValue )
	{
		getNativeElement()->SetAttribute( pName, pValue );
	}

	XmlElement::XmlElement()
	{
	}

	XmlElement::~XmlElement()
	{
	}

	tinyxml2::XMLElement* XmlElement::getNativeElement()
	{
		TIKI_ASSERT( getNativeNode()->ToElement() != nullptr );
		return ( tinyxml2::XMLElement* )getNativeNode();
	}

	const tinyxml2::XMLElement* XmlElement::getNativeElement() const
	{
		TIKI_ASSERT( getNativeNode()->ToElement() != nullptr );
		return ( tinyxml2::XMLElement* )getNativeNode();
	}
}
