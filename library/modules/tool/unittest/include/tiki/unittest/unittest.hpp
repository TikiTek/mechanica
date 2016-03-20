#pragma once
#ifndef __TIKI_UNITTTEST_HPP_INCLUDED__
#define __TIKI_UNITTTEST_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/container/list.hpp"

namespace tiki
{
	typedef bool(*UnitTestFunction)();

	namespace unittest
	{
		void	beginUnitTest( const char* pName );
		void	addTest( const char* pTitle, const char* pFile, const char* pLine, UnitTestFunction pFunc );

		int		run();
	}
}

#define TIKI_UNITTEST_PREMAINCODE( name, code ) static struct TIKI_CONCAT( UnitTestCode, name ) { TIKI_CONCAT( UnitTestCode, name ) ## () { code } } TIKI_CONCAT( s_unitTestVar, name )

#define TIKI_BEGIN_UNITTEST( name ) TIKI_UNITTEST_PREMAINCODE( name, unittest::beginUnitTest( #name ); )

#define TIKI_ADD_TEST( func_name )		\
	bool func_name	## ();				\
	TIKI_UNITTEST_PREMAINCODE(			\
		func_name,						\
		unittest::addTest(				\
			#func_name,					\
			TIKI_STRING( __FILE__ ),	\
			TIKI_STRING( __LINE__ ),	\
			func_name					\
		);								\
	);									\
	bool func_name ## ()

#endif // __TIKI_UNITTTEST_HPP_INCLUDED__
