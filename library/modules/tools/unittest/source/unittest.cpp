
#include "tiki/unittest/unittest.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/toolbase/list.hpp"

namespace tiki
{
	struct Test
	{
		const char*			pTitle;
		const char*			pFile;
		const char*			pLine;

		UnitTestFunction	pFunc;
	};

	struct UnitTest
	{
		const char*		pName;
		List< Test >	tests;
	};

	struct UnitTestSystem
	{
		List< UnitTest >	unitTests;
	};

	UnitTestSystem* getUnitTestSystem()
	{
		static UnitTestSystem system;
		return &system;
	}

	void unittest::beginUnitTest( const char* pName )
	{
		UnitTest& unitTest = getUnitTestSystem()->unitTests.add();
		unitTest.pName = pName;
	}

	void unittest::addTest( const char* pTitle, const char* pFile, const char* pLine, UnitTestFunction pFunc )
	{
		Test& test = getUnitTestSystem()->unitTests.getLast()->tests.add();
		test.pTitle		= pTitle;
		test.pFile		= pFile;
		test.pLine		= pLine;
		test.pFunc		= pFunc;
	}

	int unittest::run()
	{
		TIKI_TRACE( "Start UnitTests...\n" );

		uint testCount = 0u;
		uint testSuccess = 0u;

		for (uint i = 0u; i < getUnitTestSystem()->unitTests.getCount(); ++i)
		{
			const UnitTest& unitTest = getUnitTestSystem()->unitTests[ i ];

			TIKI_TRACE( "Run UnitTest: %s\n", unitTest.pName );
			for (uint j = 0u; j < unitTest.tests.getCount(); ++j)
			{
				const Test& test = unitTest.tests[ j ];
				TIKI_TRACE( "\t%s...", test.pTitle );

				testCount++;

				if ( test.pFunc() )
				{
					TIKI_TRACE( " OK.\n" );
					testSuccess++;
				}
				else
				{
					TIKI_TRACE( " FAILED.\n" );
					TIKI_TRACE( "%s(%s): %s failed.\n", test.pFile, test.pLine, test.pTitle );
				}
			}
		}

		getUnitTestSystem()->unitTests.dispose();

		TIKI_TRACE( "\nUnitTests finish: %u of %u success.\n===================================\n", testSuccess, testCount );
		return 0u;
	}
}