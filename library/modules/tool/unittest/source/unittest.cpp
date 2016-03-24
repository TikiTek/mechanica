
#include "tiki/unittest/unittest.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/basicstring.hpp"
#include "tiki/container/list.hpp"

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

		string				currentLog;
		bool				currentResult;
	};

	UnitTestSystem& getUnitTestSystem()
	{
		static UnitTestSystem system;
		return system;
	}

	void unittest::beginUnitTest( const char* pName )
	{
		UnitTest& unitTest = getUnitTestSystem().unitTests.add();
		unitTest.pName = pName;
	}

	void unittest::addTest( const char* pTitle, const char* pFile, const char* pLine, UnitTestFunction pFunc )
	{
		Test& test = getUnitTestSystem().unitTests.getLast().tests.add();
		test.pTitle		= pTitle;
		test.pFile		= pFile;
		test.pLine		= pLine;
		test.pFunc		= pFunc;
	}

	void unittest::addFailure( const char* pName )
	{
		if( debug::isDebuggerActive() )
		{
			debug::breakPoint();
		}

		getUnitTestSystem().currentLog += formatString( "[unittest] Test failed: %s\n", pName );
		getUnitTestSystem().currentResult = false;
	}

	int unittest::run()
	{
		debug::trace( "Start UnitTests...\n" );

		uint testCount = 0u;
		uint testSuccess = 0u;

		UnitTestSystem& system = getUnitTestSystem();
		List< UnitTest >& tests = system.unitTests;
		for (uint i = 0u; i < tests.getCount(); ++i)
		{
			const UnitTest& unitTest = tests[ i ];

			debug::trace( "Run UnitTest: %s\n", unitTest.pName );
			for (uint j = 0u; j < unitTest.tests.getCount(); ++j)
			{
				const Test& test = unitTest.tests[ j ];
				debug::trace( "\t%s...", test.pTitle );

				testCount++;

				system.currentLog = "";
				system.currentResult = true;

				bool testResult = test.pFunc();
				testResult &= system.currentResult;

				if ( testResult )
				{
					debug::trace( " OK.\n" );
					testSuccess++;
				}
				else
				{
					debug::trace( " FAILED.\n" );
					debug::trace( system.currentLog.cStr() );
					debug::trace( "%s(%s): %s failed.\n", test.pFile, test.pLine, test.pTitle );
				}
			}
		}

		tests.dispose();

		debug::trace( "\nUnitTests finish: %u of %u success.", testSuccess, testCount );
		debug::trace( "\n=====================================\n", testSuccess, testCount );
		return testCount - testSuccess;
	}
}