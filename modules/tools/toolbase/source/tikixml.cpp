
#include "tiki/base/file.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/toolbase/tikixml.hpp"

namespace tiki
{
	TikiXml* TikiXml::s_pAllocInst = nullptr;

	//typedef void(*XmlErrorHandler)(const char* _errorMessage, const char* _begin, const char* _current );
	
	void* xmlAlloc(size_t _bytes)
	{
		TikiXml* pXml	= TikiXml::s_pAllocInst;
		pXml->m_pData	= memory::allocAlign( _bytes, 0u );

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

	void TikiXml::create( const string& fileName )
	{
		Array< uint8 > data;
		TIKI_VERIFY( file::readAllBytes( fileName, data ) );

		s_pAllocInst = this;
		m_pNode = xml_create(
			(const char*)data.getData(),
			(const char*)data.getEnd(),
			xmlErrorHandler,
			xmlAlloc,
			nullptr
		);

		data.dispose();
	}

	void TikiXml::dispose()
	{
		memory::freeAlign( m_pData );

		m_pNode			= nullptr;
		m_pData			= nullptr;
	}

	const XmlElement* TikiXml::findNodeByName( const string& name ) const
	{
		TIKI_ASSERT( m_pNode );
		return xml_element_find_any( m_pNode, name.cStr() );
	}

	const XmlElement* TikiXml::findFirstChild( const string& tag, const XmlElement* pElement ) const
	{
		TIKI_ASSERT( m_pNode );
		TIKI_ASSERT( pElement );
		TIKI_ASSERT( pElement->elements );
		return xml_element_find_element( (XmlElement*)pElement, tag.cStr(), nullptr );
	}

	const XmlElement* TikiXml::findNext( const XmlElement* pElement ) const
	{
		TIKI_ASSERT( pElement );
		return xml_element_find_element( nullptr, pElement->name, (XmlElement*)pElement );
	}

	const XmlElement* TikiXml::findNext( const string& name, const XmlElement* pElement ) const
	{
		TIKI_ASSERT( pElement );
		return xml_element_find_element( nullptr, name.cStr(), (XmlElement*)pElement );
	}

	const XmlAttribute* TikiXml::findAttributeByName( const string& name, const XmlElement* pElement ) const
	{
		TIKI_ASSERT( pElement );
		return xml_element_find_attribute( (XmlElement*)pElement, name.cStr(), nullptr );
	}

	size_t TikiXml::getChilds( List< const XmlElement* >& targetList, const XmlElement* pElement, const string& name ) const
	{
		TIKI_ASSERT( pElement );
		const XmlElement* pNode = findFirstChild( name, pElement );

		while ( pNode )
		{
			targetList.add( pNode );
			pNode = findNext( name, pNode );
		}

		return targetList.getCount();
	}

}