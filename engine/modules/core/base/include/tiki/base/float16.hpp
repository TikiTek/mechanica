#pragma once
#ifndef TIKI_FLOAT16_HPP_INCLUDED
#define TIKI_FLOAT16_HPP_INCLUDED

#include "tiki/base/types.hpp"

namespace tiki
{
	namespace f16
	{
		float16 convertFloat32to16( float32 value );
		float32 convertFloat16to32( float16 value );
	}
}

#endif // TIKI_FLOAT16_HPP_INCLUDED
