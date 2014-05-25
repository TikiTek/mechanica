#pragma once
#ifndef __TIKI_COINCOMPONENT_INITDATA_HPP_INCLUDED__
#define __TIKI_COINCOMPONENT_INITDATA_HPP_INCLUDED__

#include "tiki/base/reflection.hpp"

namespace tiki
{
	TIKI_REFLECTION_STRUCT(
		CoinComponentInitData,
		TIKI_REFLECTION_FIELD( float, value )
	);
}

#endif // __TIKI_COINCOMPONENT_INITDATA_HPP_INCLUDED__
