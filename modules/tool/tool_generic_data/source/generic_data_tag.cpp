#include "tiki/tool_generic_data/generic_data_tag.hpp"

namespace tiki
{
	GenericDataTag::GenericDataTag()
	{
		m_pChildTag = nullptr;
	}

	GenericDataTag::GenericDataTag( const GenericDataTag* pCopyFrom )
	{
		if( pCopyFrom->getChildTag() != nullptr )
		{
			m_pChildTag = new GenericDataTag( pCopyFrom->getChildTag() );
		}

		m_tag		= pCopyFrom->getTag();
		m_content	= pCopyFrom->getContent();
	}

	GenericDataTag::~GenericDataTag()
	{
		if( m_pChildTag != nullptr )
		{
			TIKI_DELETE( m_pChildTag );
			m_pChildTag = nullptr;
		}
	}

	const string& GenericDataTag::getTag() const
	{
		return m_tag;
	}

	void GenericDataTag::setTag( const string& tag )
	{
		m_tag = tag;
	}

	const string& GenericDataTag::getContent() const
	{
		return m_content;
	}

	void GenericDataTag::setContent( const string& content )
	{
		m_content = content;

		if( m_pChildTag != nullptr )
		{
			TIKI_DELETE( m_pChildTag );
			m_pChildTag = nullptr;
		}
	}

	GenericDataTag* GenericDataTag::getChildTag() const
	{
		return m_pChildTag;
	}

	const GenericDataTag* GenericDataTag::getChildTag()
	{
		return m_pChildTag;
	}

	void GenericDataTag::setChildTag( GenericDataTag* pChildTag )
	{
		m_content = nullptr;
		m_pChildTag = pChildTag;
	}

	bool GenericDataTag::parseTagString( const string& rawText )
	{
		string text = rawText.trim();
		if( !isTagString( text ) )
		{
			return false;
		}

		const int contentBeginIndex = text.indexOf( ' ' );
		if( contentBeginIndex < 1 )
		{
			return false;
		}

		m_tag = text.subString( 1u, contentBeginIndex - 1u );

		const string content = text.subString( contentBeginIndex + 1, text.getLength() - contentBeginIndex - 2 );
		if( isTagString( content ) )
		{
			m_pChildTag = TIKI_NEW( GenericDataTag );
			if( !m_pChildTag->parseTagString( content ) )
			{
				m_tag.clear();

				TIKI_DELETE( m_pChildTag );
				m_pChildTag = nullptr;
				return false;
			}
		}
		else
		{
			m_content = content;
		}

		return true;
	}

	string GenericDataTag::writeTagString() const
	{
		TIKI_ASSERT( !m_tag.isEmpty() );
		TIKI_ASSERT( !m_content.isEmpty() || m_pChildTag != nullptr );

		if( m_pChildTag != nullptr )
		{
			const string childString = m_pChildTag->writeTagString();
			return formatDynamicString( "{%s %s}", m_tag.cStr(), childString.cStr() );
		}
		else
		{
			return formatDynamicString( "{%s %s}", m_tag.cStr(), m_content.cStr() );
		}
	}

	/*static*/ bool GenericDataTag::isTagString( const string& rawText )
	{
		string text = rawText.trim();
		return text.getLength() > 4u && text[ 0u ] == '{' && text[ 1u ] != '{' && text[ text.getLength() - 1 ] == '}';
	}
}
