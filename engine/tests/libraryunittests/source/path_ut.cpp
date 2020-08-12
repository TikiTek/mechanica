#include "tiki/unittest/unittest.hpp"

#include "tiki/base/path.hpp"
#include "tiki/base/string.hpp"

namespace tiki
{
	static const char* pTestPathes[] =
	{
		"C:\\directory\\name/filename.extension",
		"/directory/name/filename.extension",
		"C:\\directory\\name/filename",
		"C:\\directory\\name/.extension",
		"filename.extension",
		"",
	};

	static const char* pTestCombinePathes[][ 2u ] =
	{
		{ "C:\\directory\\name/",			"filename.extension" },
		{ "/directory/name",				"filename.extension" },
		{ "/directory/name",				"../filename.extension" },
		{ "/directory/name",				"/filename.extension" },
		{ "C:\\directory\\name/",			"filename" },
		{ "C:\\directory\\name",			".extension" },
		{ "",								"filename.extension" },
		{ "",								"" }
	};

	TIKI_BEGIN_UNITTEST( Path );

	TIKI_ADD_TEST( PathCreate )
	{
		Path path;

		path.setCompletePath( pTestPathes[ 0u ] );
		TIKI_UT_CHECK( isStringEquals( path.getPrefix(), "C:" ) );
		TIKI_UT_CHECK( isStringEquals( path.getDirectory(), "directory/name" ) );
		TIKI_UT_CHECK( isStringEquals( path.getFilename(), "filename" ) );
		TIKI_UT_CHECK( isStringEquals( path.getExtension(), ".extension" ) );

		path.setCompletePath( pTestPathes[ 1u ] );
		TIKI_UT_CHECK( isStringEmpty( path.getPrefix() ) );
		TIKI_UT_CHECK( isStringEquals( path.getDirectory(), "directory/name" ) );
		TIKI_UT_CHECK( isStringEquals( path.getFilename(), "filename" ) );
		TIKI_UT_CHECK( isStringEquals( path.getExtension(), ".extension" ) );

		path.setCompletePath( pTestPathes[ 2u ] );
		TIKI_UT_CHECK( isStringEquals( path.getPrefix(), "C:" ) );
		TIKI_UT_CHECK( isStringEquals( path.getDirectory(), "directory/name" ) );
		TIKI_UT_CHECK( isStringEquals( path.getFilename(), "filename" ) );
		TIKI_UT_CHECK( isStringEmpty( path.getExtension() ) );

		path.setCompletePath( pTestPathes[ 3u ] );
		TIKI_UT_CHECK( isStringEquals( path.getPrefix(), "C:" ) );
		TIKI_UT_CHECK( isStringEquals( path.getDirectory(), "directory/name" ) );
		TIKI_UT_CHECK( isStringEmpty( path.getFilename() ) );
		TIKI_UT_CHECK( isStringEquals( path.getExtension(), ".extension" ) );

		path.setCompletePath( pTestPathes[ 4u ] );
		TIKI_UT_CHECK( isStringEmpty( path.getPrefix() ) );
		TIKI_UT_CHECK( isStringEmpty( path.getDirectory() ) );
		TIKI_UT_CHECK( isStringEquals( path.getFilename(), "filename" ) );
		TIKI_UT_CHECK( isStringEquals( path.getExtension(), ".extension" ) );

		path.setCompletePath( pTestPathes[ 5u ] );
		TIKI_UT_CHECK( isStringEmpty( path.getPrefix() ) );
		TIKI_UT_CHECK( isStringEmpty( path.getDirectory() ) );
		TIKI_UT_CHECK( isStringEmpty( path.getFilename() ) );
		TIKI_UT_CHECK( isStringEmpty( path.getExtension() ) );
	}

	TIKI_ADD_TEST( PathCombine )
	{
		Path path;

		path.setCombinedPath( pTestCombinePathes[ 0u ][ 0u ], pTestCombinePathes[ 0u ][ 1u ] );
		TIKI_UT_CHECK( isStringEquals( path.getPrefix(), "C:" ) );
		TIKI_UT_CHECK( isStringEquals( path.getDirectory(), "directory/name" ) );
		TIKI_UT_CHECK( isStringEquals( path.getFilename(), "filename" ) );
		TIKI_UT_CHECK( isStringEquals( path.getExtension(), ".extension" ) );

		path.setCombinedPath( pTestCombinePathes[ 1u ][ 0u ], pTestCombinePathes[ 1u ][ 1u ] );
		TIKI_UT_CHECK( isStringEmpty( path.getPrefix() ) );
		TIKI_UT_CHECK( isStringEquals( path.getDirectory(), "directory/name" ) );
		TIKI_UT_CHECK( isStringEquals( path.getFilename(), "filename" ) );
		TIKI_UT_CHECK( isStringEquals( path.getExtension(), ".extension" ) );

		path.setCombinedPath( pTestCombinePathes[ 2u ][ 0u ], pTestCombinePathes[ 2u ][ 1u ] );
		TIKI_UT_CHECK( isStringEquals( path.getPrefix(), "C:" ) );
		TIKI_UT_CHECK( isStringEquals( path.getDirectory(), "directory/name" ) );
		TIKI_UT_CHECK( isStringEquals( path.getFilename(), "filename" ) );
		TIKI_UT_CHECK( isStringEquals( path.getExtension(), ".extension" ) );

		path.setCombinedPath( pTestCombinePathes[ 3u ][ 0u ], pTestCombinePathes[ 3u ][ 1u ] );
		TIKI_UT_CHECK( isStringEquals( path.getPrefix(), "C:" ) );
		TIKI_UT_CHECK( isStringEquals( path.getDirectory(), "directory/name" ) );
		TIKI_UT_CHECK( isStringEquals( path.getFilename(), "filename" ) );
		TIKI_UT_CHECK( isStringEquals( path.getExtension(), ".extension" ) );

		path.setCombinedPath( pTestCombinePathes[ 4u ][ 0u ], pTestCombinePathes[ 4u ][ 1u ] );
		TIKI_UT_CHECK( isStringEquals( path.getPrefix(), "C:" ) );
		TIKI_UT_CHECK( isStringEquals( path.getDirectory(), "directory/name" ) );
		TIKI_UT_CHECK( isStringEquals( path.getFilename(), "filename" ) );
		TIKI_UT_CHECK( isStringEquals( path.getExtension(), ".extension" ) );

		path.setCombinedPath( pTestCombinePathes[ 5u ][ 0u ], pTestCombinePathes[ 5u ][ 1u ] );
		TIKI_UT_CHECK( isStringEquals( path.getPrefix(), "C:" ) );
		TIKI_UT_CHECK( isStringEquals( path.getDirectory(), "directory/name" ) );
		TIKI_UT_CHECK( isStringEquals( path.getFilename(), "filename" ) );
		TIKI_UT_CHECK( isStringEquals( path.getExtension(), ".extension" ) );

		path.setCombinedPath( pTestCombinePathes[ 6u ][ 0u ], pTestCombinePathes[ 6u ][ 1u ] );
		TIKI_UT_CHECK( isStringEquals( path.getPrefix(), "C:" ) );
		TIKI_UT_CHECK( isStringEquals( path.getDirectory(), "directory/name" ) );
		TIKI_UT_CHECK( isStringEquals( path.getFilename(), "filename" ) );
		TIKI_UT_CHECK( isStringEquals( path.getExtension(), ".extension" ) );

		path.setCombinedPath( pTestCombinePathes[ 7u ][ 0u ], pTestCombinePathes[ 7u ][ 1u ] );
		TIKI_UT_CHECK( isStringEmpty( path.getPrefix() ) );
		TIKI_UT_CHECK( isStringEmpty( path.getDirectory() ) );
		TIKI_UT_CHECK( isStringEmpty( path.getFilename() ) );
		TIKI_UT_CHECK( isStringEmpty( path.getExtension() ) );
	}
}
