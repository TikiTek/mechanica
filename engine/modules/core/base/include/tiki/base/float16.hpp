#pragma once
#ifndef TIKI_FLOAT16_HPP__INCLUDED
#define TIKI_FLOAT16_HPP__INCLUDED

#include "tiki/base/types.hpp"

namespace tiki
{
	namespace f16
	{
		float16 convertFloat32to16( float value );
		float convertFloat16to32( float16 value );
	}
}

#endif // TIKI_FLOAT16_HPP__INCLUDED
