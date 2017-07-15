#pragma once
#ifndef TIKI_XML_ATTRIBUTE_HPP_INCLUDED
#define TIKI_XML_ATTRIBUTE_HPP_INCLUDED

namespace tinyxml2
{
	class XMLAttribute;
}

namespace tiki
{
	class XmlNode;

	class XmlAttribute
	{
	public:

		const char*			getName() const;
		void				setName( const char* pValue );
		const char*			getValue() const;
		void				setValue( const char* pValue );

		XmlNode*			getParent();
		const XmlNode*		getParent() const;

		XmlAttribute*		getPreviousAttribute();
		const XmlAttribute*	getPreviousAttribute() const;
		XmlAttribute*		getNextAttribute();
		const XmlAttribute*	getNextAttribute() const;


	private:

								XmlAttribute();
								~XmlAttribute();

		tinyxml2::XMLAttribute*	getNativeAttribute();
	};
}

#endif // TIKI_XML_ATTRIBUTE_HPP_INCLUDED
