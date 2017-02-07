#include "tiki/toolgenericdata/genericdatavaluetag.hpp"

namespace tiki
{
	GenericDataValueTag::GenericDataValueTag( GenericDataTypeCollection& typeCollection )
		: m_typeCollection( typeCollection )
	{
		m_pChildTag = nullptr;
	}

	GenericDataValueTag::~GenericDataValueTag()
	{
		TIKI_DELETE( m_pChildTag );
		m_pChildTag = nullptr;
	}

	const string& GenericDataValueTag::getTag() const
	{
		return m_tag;
	}

	void GenericDataValueTag::setTag( const string& tag )
	{
		m_tag = tag;
	}

	const string& GenericDataValueTag::getContent() const
	{
		return m_content;
	}

	void GenericDataValueTag::setContent( const string& content )
	{
		m_content = content;
		TIKI_DELETE( m_pChildTag );
		m_pChildTag = nullptr;
	}

	GenericDataValueTag* GenericDataValueTag::getChildTag() const
	{
		return m_pChildTag;
	}

	const GenericDataValueTag* GenericDataValueTag::getChildTag()
	{
		return m_pChildTag;
	}

	void GenericDataValueTag::setChildTag( GenericDataValueTag* pChildTag )
	{
		m_content = nullptr;
		m_pChildTag = pChildTag;
	}

	bool GenericDataValueTag::parseTagString( const string& text )
	{
		return false;
	}

	string GenericDataValueTag::writeTagString() const
	{
		return "";
	}
}
