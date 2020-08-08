#pragma once

#include "tiki/base/types.hpp"

namespace tiki
{
	namespace f16
	{
		float16 convertFloat32to16( float32 value );
		float32 convertFloat16to32( float16 value );
	}
}
