
#include "tiki/unittest/unittest.hpp"

#include "tiki/container/sortedsizedmap.hpp"

namespace tiki
{	
	TIKI_BEGIN_UNITTEST( SortedSizedMap );

	TIKI_ADD_TEST( SortedSizedMapIntert )
	{
		const uintreg count	= 10u;
		const uintreg maxIndex	= count - 1u;

		SortedSizedMap< uintreg, uintreg > map;
		map.create( count );

		for (uintreg i = 0u; i < count; ++i)
		{
			map.set( maxIndex - i, i );
		}

		for (uintreg i = 0u; i < count; ++i)
		{
			uintreg value = TIKI_SIZE_T_MAX;
			map.findValue( &value, i );

			TIKI_UT_CHECK( value == (maxIndex - i) );
		}

		map.dispose();
	}
}