#pragma once
#ifndef TIKI_DATABLOCK_HPP__INCLUDED
#define TIKI_DATABLOCK_HPP__INCLUDED

#include "tiki/base/types.hpp"

namespace tiki
{
	enum AnimationDataBlobArrayType
	{
		DataType_UInt8,
		DataType_SInt8,
		DataType_UInt16,
		DataType_SInt16,
		DataType_UInt32,
		DataType_SInt32,
		DataType_UInt64,
		DataType_SInt64
	};

	struct AnimationDataBlobArray
	{
		size_t						alignment;

		AnimationDataBlobArrayType	type;
		size_t						typeSize;

		uint8*						pData;
		size_t						elementCount;
	};
}

#endif // TIKI_DATABLOCK_HPP__INCLUDED

