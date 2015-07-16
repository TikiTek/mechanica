
#include "tiki/genericdata/genericdatadocument.hpp"

#include "tiki/io/xmlreader.hpp"

namespace tiki
{
	GenericDataDocument::GenericDataDocument()
	{
	}

	GenericDataDocument::~GenericDataDocument()
	{

	}

	void GenericDataDocument::addObject( const string& pName, GenericDataObject* pObject )
	{

	}

	GenericDataObject* GenericDataDocument::getObject()
	{
		return nullptr;
	}

	const GenericDataObject* GenericDataDocument::getObject() const
	{
		return nullptr;
	}

	bool GenericDataDocument::importFromXml( XmlReader& reader )
	{
		const XmlElement* pRootNode = reader.findNodeByName( "tikigenericobjects" );
		if ( !pRootNode )
		{
			TIKI_TRACE_ERROR( "[GenericDataDocument::importFromXml] Unable to find root node.\n" );
			return false;
		}

		return true;
	}

	bool GenericDataDocument::exportToResource( ResourceWriter& writer ) const
	{
		return true;
	}
}
