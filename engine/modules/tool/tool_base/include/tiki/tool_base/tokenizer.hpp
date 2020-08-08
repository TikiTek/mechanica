#pragma once

#include "tiki/base/dynamic_string.hpp"

namespace tiki
{
	class Tokenizer
	{
	public:

		void			create( const DynamicString& text, const DynamicString& token, bool tokenIsCharList = false );
		void			dispose();

		DynamicString	findNext( uint* pIndex = nullptr );

	private:

		DynamicString	m_text;
		DynamicString	m_token;

		bool			m_tokenAsChars;

		uint			m_currentIndex;

	};
}
