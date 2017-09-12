#include "tiki/toolxml/xml_attribute.hpp"

#include <tinyxml2.h>

namespace tiki
{
	const char* XmlAttribute::getName() const
	{
		return getNativeAttribute()->Name();
	}

	const char* XmlAttribute::getValue() const
	{
		return getNativeAttribute()->Value();
	}

	XmlAttribute::XmlAttribute()
	{
	}

	XmlAttribute::~XmlAttribute()
	{
	}

	const tinyxml2::XMLAttribute* XmlAttribute::getNativeAttribute() const
	{
		return ( const tinyxml2::XMLAttribute* )this;
	}
}
