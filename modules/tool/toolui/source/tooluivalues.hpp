#pragma once
#ifndef TIKI_TOOLUIVALUES_HPP_INCLUDED
#define TIKI_TOOLUIVALUES_HPP_INCLUDED

#include "tiki/graphics/color.hpp"

namespace tiki
{
	static const Color	s_border			= TIKI_COLOR( 0x05u, 0x05u, 0x08u, 0xffu );

	static const Color	s_backgroundLight	= TIKI_COLOR( 0x10u, 0x10u, 0x10u, 0xffu );
	static const Color	s_backgroundDefault	= TIKI_COLOR( 0x09u, 0x09u, 0x09u, 0xffu );
	static const Color	s_backgroundDark	= TIKI_COLOR( 0x07u, 0x07u, 0x07u, 0xffu );

	static const Color	s_hightlightLight	= TIKI_COLOR( 0x66u, 0xaau, 0xffu, 0xffu );
	static const Color	s_hightlightDefault	= TIKI_COLOR( 0x44u, 0x88u, 0xbbu, 0xffu );
	static const Color	s_hightlightDark	= TIKI_COLOR( 0x22u, 0x66u, 0x99u, 0xffu );
}

#endif // TIKI_TOOLUIVALUES_HPP_INCLUDED
