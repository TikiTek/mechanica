
#include "tiki/unittest/unittest.hpp"

#include "tiki/container/sortedsizedmap.hpp"

namespace tiki
{	
	TIKI_BEGIN_UNITTEST( SortedSizedMap );

	TIKI_ADD_TEST( SortedSizedMapIntert )
	{
		const uint count	= 10u;
		const uint maxIndex	= count - 1u;

		SortedSizedMap< uint, uint > map;
		map.create( count );

		for (uint i = 0u; i < count; ++i)
		{
			map.set( maxIndex - i, i );
		}

		for (uint i = 0u; i < count; ++i)
		{
			uint value = TIKI_SIZE_T_MAX;
			map.findValue( &value, i );

			TIKI_UT_CHECK( value == (maxIndex - i) );
		}

		map.dispose();
	}
}