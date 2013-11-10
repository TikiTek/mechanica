#pragma once
#ifndef __TIKI_ENDIANNESS_HPP_INCLUDED__
#define __TIKI_ENDIANNESS_HPP_INCLUDED__

#include "tiki/base/platform.hpp"

namespace tiki
{
	enum Endianness
	{
		Endianness_Invalid = -1,

		Endianness_Little,
		Endianness_MiddleEndian,
		Endianness_Big,

		Endianness_Count
	};

	Endianness		getHostEndianness();
	Endianness		getPlatformEndianness( PlatformType platform );

	void			changeEndianness( void* pData, uint size );
	void			checkAndChangeEndianness( void* pData, uint size, PlatformType targetPlatform );

}

#endif // __TIKI_ENDIANNESS_HPP_INCLUDED__
