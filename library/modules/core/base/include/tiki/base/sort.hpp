#pragma once
#ifndef __TIKI_SORT_HPP_INCLUDED__
#define __TIKI_SORT_HPP_INCLUDED__

#include "tiki/base/types.hpp"

namespace tiki
{
	template<typename T>
	void quickSort( T* pData, uint capacity )
	{
		quickSort( pData, capacity, 0u, capacity - 1u );
	}

	template<typename T>
	void quickSort( T* pData, uint capacity, uint left, uint right )
	{
		uint i = left;
		uint j = right;

		while ( right > left )
		{
			i = left;
			j = right - 1u;
			uint p = right;
			T value = pData[ p ];

			do
			{
				while ( pData[ i ] < value && i < right )
				{
					i++;
				}

				while ( pData[ j ] >= value && j > left )
				{
					j--;
				}

				if (i < j)
				{
					T valueTemp = pData[ i ];
					pData[ i ] = pData[ j ];
					pData[ j ] = valueTemp;
				}
			} while ( i < j );

			if ( pData[ i ] > value )
			{
				T valueTemp = pData[ i ];
				pData[ i ]		= pData[ right ];
				pData[ right ]	= valueTemp;
			}

			if ( j > i )
			{
				quickSort( pData, capacity, left, i - 1 );
				left = i + 1;
			}
			else
			{
				quickSort( pData, capacity, i + 1, right );
				right = i - 1;
			}
		}
	}

	template<typename T>
	uint quickSortSeperate( T* pData, uint capacity, uint start, uint end, uint& i, uint& j )
	{
		return i;
	}
}

#endif // __TIKI_SORT_HPP_INCLUDED__
