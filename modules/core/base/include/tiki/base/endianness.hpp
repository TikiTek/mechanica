#pragma once
#ifndef __TIKI_ENDIANNESS_HPP_INCLUDED__
#define __TIKI_ENDIANNESS_HPP_INCLUDED__

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
}

#endif // __TIKI_ENDIANNESS_HPP_INCLUDED__
