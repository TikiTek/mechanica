#pragma once
#ifndef __TIKI_STOCKVERTEX_HPP_INCLUDED__
#define __TIKI_STOCKVERTEX_HPP_INCLUDED__

#include "tiki/base/structs.hpp"

namespace tiki
{
	struct StockVertexPos2
	{
		float2	position;
	};

	struct StockVertexPos2Tex2
	{
		float2	position;
		float2	texCoord;
	};

	struct StockVertexPos3
	{
		float3	position;
	};

	struct StockVertexPos3Tex2
	{
		float3	position;
		float2	texCoord;
	};
}

#endif // __TIKI_STOCKVERTEX_HPP_INCLUDED__
