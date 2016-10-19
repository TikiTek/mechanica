
#include "tiki/io/xmlreader.hpp"

#include "tiki/base/memory.hpp"
#include "tiki/io/file.hpp"

namespace tiki
{
	XmlReader::XmlReader()
	{
		m_pNode			= nullptr;
		m_pData			= nullptr;
	}
	
	XmlReader::~XmlReader()
	{
		TIKI_ASSERT( m_pNode == nullptr );
		TIKI_ASSERT( m_pData == nullptr );
	}

	bool XmlReader::create( const char* pFileName )
	{
		Array< uint8 > data;
		if( !file::readAllBytes( pFileName, data ) )
		{
			return false;
		}

		m_pNode = xml_create(
			(const char*)data.getBegin(),
			(const char*)data.getEnd(),
			&errorHandler,
			&allocateMemory,
			nullptr,
			this
		);

		data.dispose();
		
		if ( m_pNode == nullptr )
		{
			dispose();
			return false;
		}

		return true;
	}

	void XmlReader::dispose()
	{
		TIKI_MEMORY_FREE( m_pData );

		m_pNode	= nullptr;
		m_pData	= nullptr;
	}

	const XmlElement* XmlReader::findNodeByName( const char* pName ) const
	{
		TIKI_ASSERT( m_pNode );
		return xml_element_find_any( m_pNode, pName );
	}

	const XmlElement* XmlReader::findFirstChild( const char* pName, const XmlElement* pElement ) const
	{
		TIKI_ASSERT( m_pNode );
		TIKI_ASSERT( pElement );

		return xml_element_find_element( (XmlElement*)pElement, pName, nullptr );
	}

	const XmlElement* XmlReader::findNext( const XmlElement* pElement ) const
	{
		TIKI_ASSERT( pElement );
		return xml_element_find_element( nullptr, pElement->name, (XmlElement*)pElement );
	}

	const XmlElement* XmlReader::findNext( const char* pName, const XmlElement* pElement ) const
	{
		TIKI_ASSERT( pElement );
		return xml_element_find_element( nullptr, pName, (XmlElement*)pElement );
	}

	const XmlAttribute* XmlReader::findAttributeByName( const char* pName, const XmlElement* pElement ) const
	{
		TIKI_ASSERT( pElement );
		return xml_element_find_attribute( (XmlElement*)pElement, pName, nullptr );
	}

	size_t XmlReader::getChilds( XmlElementList& targetList, const XmlElement* pElement, const char* pName ) const
	{
		TIKI_ASSERT( pElement );
		const uint count = xml_element_find_elements( (XmlElement*)pElement, pName, nullptr, nullptr );
		targetList.create( count );
		xml_element_find_elements( (XmlElement*)pElement, pName, (XmlElement**)targetList.getBegin(), (XmlElement**)targetList.end() );

		return targetList.getCount();
	}

	/*static*/ void* XmlReader::allocateMemory( ::size_t _bytes, void* pUserData )
	{
		XmlReader* pXml	= static_cast<XmlReader*>(pUserData);
		pXml->m_pData	= TIKI_ALLOC( _bytes );

		if( pXml->m_pData == nullptr )
		{
			TIKI_TRACE_ERROR( "[xml] Inable to allocate memory for XmlReader!\n" );
		}

		return pXml->m_pData;
	}

	/*static*/ void XmlReader::errorHandler( const char* pErrorMessage, const char* pBegin, const char* pCurrent )
	{
		TIKI_TRACE_ERROR( pErrorMessage );
	}
}