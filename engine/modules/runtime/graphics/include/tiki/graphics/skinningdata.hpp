#pragma once
#ifndef __TIKI_SKINNINGDATA_HPP_INCLUDED__
#define __TIKI_SKINNINGDATA_HPP_INCLUDED__

#include "tiki/graphics/constantbuffer.hpp"

namespace tiki
{
	// TODO: find a better solution
	struct SkinningData
	{
		ConstantBuffer	matrices;
	};
}

#endif // __TIKI_SKINNINGDATA_HPP_INCLUDED__
