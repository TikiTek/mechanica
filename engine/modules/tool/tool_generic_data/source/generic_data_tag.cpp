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

	const DynamicString& GenericDataTag::getTag() const
	{
		return m_tag;
	}

	void GenericDataTag::setTag( const DynamicString& tag )
	{
		m_tag = tag;
	}

	const DynamicString& GenericDataTag::getContent() const
	{
		return m_content;
	}

	void GenericDataTag::setContent( const DynamicString& content )
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

	bool GenericDataTag::parseTagString( const DynamicString& rawText )
	{
		DynamicString text = rawText.trim();
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

		const DynamicString content = text.subString( contentBeginIndex + 1, text.getLength() - contentBeginIndex - 2 );
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

	DynamicString GenericDataTag::writeTagString() const
	{
		if( m_pChildTag != nullptr )
		{
			const DynamicString childString = m_pChildTag->writeTagString();
			return formatDynamicString( "{%s %s}", m_tag.cStr(), childString.cStr() );
		}
		else if ( !m_tag.isEmpty() && !m_content.isEmpty() )
		{
			return formatDynamicString( "{%s %s}", m_tag.cStr(), m_content.cStr() );
		}

		return "{Invalid Tag}";
	}

	/*static*/ bool GenericDataTag::isTagString( const DynamicString& rawText )
	{
		DynamicString text = rawText.trim();
		return text.getLength() > 4u && text[ 0u ] == '{' && text[ 1u ] != '{' && text[ text.getLength() - 1 ] == '}';
	}
}
