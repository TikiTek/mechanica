#include "tiki/tool_base/tokenizer.hpp"

namespace tiki
{
	void Tokenizer::create( const DynamicString& text, const DynamicString& token, bool tokenIsCharList /*= false */ )
	{
		m_text			= text;
		m_token			= token;

		m_tokenAsChars	= tokenIsCharList;

		m_currentIndex	= 0u;
	}

	void Tokenizer::dispose()
	{
		m_text		= "";
		m_token		= "";
	}

	DynamicString Tokenizer::findNext( uintreg* pIndex /*= nullptr */ )
	{
		uintreg nextIndex = TIKI_SIZE_T_MAX;

		if ( m_tokenAsChars )
		{
			bool found = false;
			uintreg index = m_currentIndex - 1u;
			const char* pText = m_text.cStr();
			do
			{
				const char tc = pText[ ++index ];

				for (uintreg i = 0u; i < m_token.getLength(); ++i)
				{
					if ( tc == m_token[ i ])
					{
						found = true;
						break;
					}
				}

				if ( found && index == m_currentIndex )
				{
					m_currentIndex++;
					found = false;
				}
			}
			while ( !found && index < m_text.getLength() );

			nextIndex = index;

			if ( index == m_text.getLength() )
			{
				nextIndex = -1;
			}
		}
		else
		{
			while ( ( nextIndex = m_text.indexOf( m_token, m_currentIndex ) ) == m_currentIndex )
			{
				m_currentIndex++;
			}
		}

		if ( pIndex != nullptr )
		{
			*pIndex = nextIndex;
		}

		if ( nextIndex == TIKI_SIZE_T_MAX )
		{
			nextIndex	= m_text.getLength();
		}

		if ( m_currentIndex >= nextIndex )
		{
			return "";
		}

		DynamicString part = m_text.subString( m_currentIndex, sint( nextIndex - m_currentIndex ) );
		m_currentIndex = nextIndex;

		return part;
	}

}