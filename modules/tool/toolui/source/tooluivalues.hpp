#pragma once
#ifndef TIKI_TOOLUIVALUES_HPP_INCLUDED
#define TIKI_TOOLUIVALUES_HPP_INCLUDED

#include "tiki/graphics/color.hpp"

namespace tiki
{
	static const Color	s_border			= TIKI_COLOR( 0x04u, 0x04u, 0x08u, 0xffu );

	static const Color	s_backgroundLight	= TIKI_COLOR( 0x33u, 0x33u, 0x33u, 0xddu );
	static const Color	s_backgroundDefault	= TIKI_COLOR( 0x22u, 0x22u, 0x22u, 0xffu );
	static const Color	s_backgroundDark	= TIKI_COLOR( 0x11u, 0x11u, 0x11u, 0xffu );

	static const Color	s_hightlightLight	= TIKI_COLOR( 0x66u, 0xaau, 0xffu, 0xffu );
	static const Color	s_hightlightDefault	= TIKI_COLOR( 0x44u, 0x88u, 0xbbu, 0xffu );
	static const Color	s_hightlightDark	= TIKI_COLOR( 0x22u, 0x66u, 0x99u, 0xffu );
}

#endif // TIKI_TOOLUIVALUES_HPP_INCLUDED
