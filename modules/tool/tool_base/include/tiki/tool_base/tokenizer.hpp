#pragma once
#ifndef TIKI_TOKENIZER_HPP
#define TIKI_TOKENIZER_HPP

#include "tiki/base/dynamic_string.hpp"

namespace tiki
{
	class Tokenizer
	{
	public:

		void	create( const string& text, const string& token, bool tokenIsCharList = false );
		void	dispose();

		string	findNext( uint* pIndex = nullptr );

	private:

		string	m_text;
		string	m_token;

		bool	m_tokenAsChars;

		uint	m_currentIndex;

	};
}

#endif // TIKI_TOKENIZER_HPP
