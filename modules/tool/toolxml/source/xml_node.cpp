#include "tiki/toolxml/xml_node.hpp"

namespace tiki
{
	XmlDocument& XmlNode::getDocument()
	{
		return *( XmlDocument* )getNativeNode()->GetDocument();
	}

	const XmlDocument& XmlNode::getDocument() const
	{
		return *( const XmlDocument* )getNativeNode()->GetDocument();
	}

	const char* XmlNode::getValue() const
	{
		return getNativeNode()->Value();
	}

	void XmlNode::setValue( const char* pValue )
	{
		getNativeNode()->SetValue( pValue );
	}

	XmlNode* XmlNode::getParent()
	{
		return ( XmlNode* )getNativeNode()->Parent();
	}

	const XmlNode* XmlNode::getParent() const
	{
		return ( XmlNode* )getNativeNode()->Parent();
	}

	XmlElement* XmlNode::getPreviousSibling()
	{
		return ( XmlElement* )getNativeNode()->PreviousSiblingElement();
	}

	const XmlElement* XmlNode::getPreviousSibling() const
	{
		return ( const XmlElement* )getNativeNode()->PreviousSiblingElement();
	}

	XmlElement* XmlNode::getNextSibling()
	{
		return ( XmlElement* )getNativeNode()->NextSiblingElement();
	}

	const XmlElement* XmlNode::getNextSibling() const
	{
		return ( const XmlElement* )getNativeNode()->NextSiblingElement();
	}

	XmlElement* XmlNode::findNextSibling( const char* pName )
	{
		return ( XmlElement* )getNativeNode()->NextSiblingElement( pName );
	}

	const XmlElement* XmlNode::findNextSibling( const char* pName ) const
	{
		return ( const XmlElement* )getNativeNode()->NextSiblingElement( pName );
	}

	XmlElement* XmlNode::getFirstChild()
	{
		return ( XmlElement* )getNativeNode()->FirstChildElement();
	}

	const XmlElement* XmlNode::getFirstChild() const
	{
		return ( const XmlElement* )getNativeNode()->FirstChildElement();
	}

	XmlElement* XmlNode::getLastChild()
	{
		return ( XmlElement* )getNativeNode()->LastChildElement();
	}

	const XmlElement* XmlNode::getLastChild() const
	{
		return ( const XmlElement* )getNativeNode()->LastChildElement();
	}

	XmlElement* XmlNode::findFirstChild( const char* pName )
	{
		return ( XmlElement* )getNativeNode()->FirstChildElement( pName );
	}

	const XmlElement* XmlNode::findFirstChild( const char* pName ) const
	{
		return ( const XmlElement* )getNativeNode()->FirstChildElement( pName );
	}

	void XmlNode::appendChild( XmlElement* pElement )
	{
		getNativeNode()->InsertEndChild( pElement->getNativeNode() );
	}

	void XmlNode::removeChild( XmlElement* pElement )
	{
		getNativeNode()->DeleteChild( pElement->getNativeNode() );
	}

	XmlNode::XmlNode()
	{
	}

	XmlNode::~XmlNode()
	{
	}

	tinyxml2::XMLNode* XmlNode::getNativeNode()
	{
		return ( tinyxml2::XMLNode* )this;
	}

	const tinyxml2::XMLNode* XmlNode::getNativeNode() const
	{
		return ( const tinyxml2::XMLNode* )this;
	}
}
