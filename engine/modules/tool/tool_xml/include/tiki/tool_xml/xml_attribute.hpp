#pragma once

namespace tinyxml2
{
	class XMLAttribute;
}

namespace tiki
{
	class XmlElement;

	class XmlAttribute
	{
	public:

		const char*						getName() const;
		const char*						getValue() const;

	private:

										XmlAttribute();
										~XmlAttribute();

		const tinyxml2::XMLAttribute*	getNativeAttribute() const;
	};
}
