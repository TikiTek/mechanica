#include "tiki/unittest/unittest.hpp"

#include "tiki/math/matrix.hpp"

namespace tiki
{
	TIKI_BEGIN_UNITTEST( Matrix43 );

	TIKI_ADD_TEST( Matrix43Mul )
	{
		const Matrix43 test1 =
		{
			{
				{ -2.0f, 0.0f, -1.0f },
				{  5.0f, 0.0f, -4.0f },
				{ -1.0f, 2.0f,  1.0f }
			},
			{  5.0f, 1.0f, -3.0f }
		};

		const Matrix43 test2 =
		{
			{
				{  3.0f,  6.0f,  4.0f },
				{  2.0f, -1.0f,  0.0f },
				{ -2.0f,  0.0f, -4.0f }
			},
			{ -3.0f, -5.0f, -1.0f }
		};


		Matrix43 test;
		matrix::mul( test, test1, test2 );

		TIKI_UT_CHECK( f32::isEquals( test.rot.x.x,  -4.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.rot.x.y, -12.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.rot.x.z,  -4.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.rot.y.x,  23.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.rot.y.y,  30.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.rot.y.z,  36.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.rot.z.x,  -1.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.rot.z.y,  -8.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.rot.z.z,  -8.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.pos.x, 20.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.pos.y, 24.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.pos.z, 31.0f, 0.001f ) );
	}

	TIKI_ADD_TEST( Matrix43Transform3 )
	{
		const Matrix43 mtx =
		{
			{
				{ -2.0f, 0.0f, -1.0f },
				{  5.0f, 0.0f, -4.0f },
				{ -1.0f, 2.0f,  1.0f }
			},
			{  5.0f, 1.0f, -3.0f }
		};

		Vector3 test = { 3.0f, 6.0f, 4.0f };
		matrix::transform( test, mtx );

		TIKI_UT_CHECK( f32::isEquals( test.x, -5.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y,  0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z, 10.0f, 0.001f ) );
	}
}