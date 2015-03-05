
#include "tiki/base/file.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/toolbase/tikixml.hpp"

namespace tiki
{
	void* xmlAlloc(size_t _bytes, void* pUserData)
	{
		TikiXml* pXml	= static_cast< TikiXml* >( pUserData );
		pXml->m_pData	= TIKI_MEMORY_ALLOC( _bytes );

		return pXml->m_pData;
	}

	void xmlErrorHandler( const char* _errorMessage, const char* _begin, const char* _current )
	{
		TIKI_TRACE_ERROR( _errorMessage );
	}

	TikiXml::TikiXml()
	{
		m_pNode			= nullptr;
		m_pData			= nullptr;
	}
	
	TikiXml::~TikiXml()
	{
		TIKI_ASSERT( m_pNode == nullptr );
		TIKI_ASSERT( m_pData == nullptr );
	}

	void TikiXml::create( cstring pFileName )
	{
		Array< uint8 > data;
		TIKI_VERIFY( file::readAllBytes( pFileName, data ) );

		m_pNode = xml_create(
			(const char*)data.getBegin(),
			(const char*)data.getEnd(),
			xmlErrorHandler,
			xmlAlloc,
			nullptr,
			this
		);

		data.dispose();
	}

	void TikiXml::dispose()
	{
		memory::freeAlign( m_pData );

		m_pNode			= nullptr;
		m_pData			= nullptr;
	}

	const XmlElement* TikiXml::findNodeByName( cstring pName ) const
	{
		TIKI_ASSERT( m_pNode );
		return xml_element_find_any( m_pNode, pName );
	}

	const XmlElement* TikiXml::findFirstChild( cstring pName, const XmlElement* pElement ) const
	{
		TIKI_ASSERT( m_pNode );
		TIKI_ASSERT( pElement );
		TIKI_ASSERT( pElement->elements );
		return xml_element_find_element( (XmlElement*)pElement, pName, nullptr );
	}

	const XmlElement* TikiXml::findNext( const XmlElement* pElement ) const
	{
		TIKI_ASSERT( pElement );
		return xml_element_find_element( nullptr, pElement->name, (XmlElement*)pElement );
	}

	const XmlElement* TikiXml::findNext( cstring pName, const XmlElement* pElement ) const
	{
		TIKI_ASSERT( pElement );
		return xml_element_find_element( nullptr, pName, (XmlElement*)pElement );
	}

	const XmlAttribute* TikiXml::findAttributeByName( cstring pName, const XmlElement* pElement ) const
	{
		TIKI_ASSERT( pElement );
		return xml_element_find_attribute( (XmlElement*)pElement, pName, nullptr );
	}

	size_t TikiXml::getChilds( XmlElementList& targetList, const XmlElement* pElement, cstring pName ) const
	{
		TIKI_ASSERT( pElement );
		const XmlElement* pNode = findFirstChild( pName, pElement );

		while ( pNode )
		{
			targetList.add( pNode );
			pNode = findNext( pName, pNode );
		}

		return targetList.getCount();
	}
}