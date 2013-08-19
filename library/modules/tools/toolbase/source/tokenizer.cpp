
#include "tiki/toolbase/tokenizer.hpp"

namespace tiki
{
	void Tokenizer::create( const string& text, const string& token, bool tokenIsCharList /*= false */ )
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

	string Tokenizer::findNext( size_t* pIndex /*= nullptr */ )
	{
		size_t nextIndex	= (size_t)-1;

		if ( m_tokenAsChars )
		{
			bool found = false;
			size_t index = m_currentIndex - 1u;
			cstring pText = m_text.cStr();
			do 
			{
				const char tc = pText[ ++index ];

				for (size_t i = 0u; i < m_token.length(); ++i)
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
			while ( !found && index < m_text.length() );

			nextIndex = index;

			if ( index == m_text.length() )
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

		if ( nextIndex == (size_t)-1 )
		{
			nextIndex	= m_text.length();
		}

		if ( m_currentIndex >= nextIndex )
		{
			return "";
		}

		string part = m_text.substring( m_currentIndex, nextIndex - m_currentIndex );
		m_currentIndex = nextIndex;

		return part;
	}

}