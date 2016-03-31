#include "tiki/unittest/unittest.hpp"

#include "tiki/math/matrix.hpp"

namespace tiki
{
	TIKI_BEGIN_UNITTEST( Matrix33 );

	TIKI_ADD_TEST( Matrix33CreateScale )
	{
		const Vector3 scale = { 5.0f, 5.0f, 5.0f };

		Matrix33 test;
		matrix::createScale( test, scale );

		TIKI_UT_CHECK( f32::isEquals( test.x.x, 5.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.x.y, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.x.z, 0.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.y.x, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y.y, 5.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y.z, 0.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.z.x, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z.y, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z.z, 5.0f, 0.001f ) );

		return true;
	}

	TIKI_ADD_TEST( Matrix33CreateRationX )
	{
		Matrix33 test;
		matrix::createRotationX( test, f32::piOver4 );

		TIKI_UT_CHECK( f32::isEquals( test.x.x, 1.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.x.y, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.x.z, 0.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.y.x, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y.y, 0.707107f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y.z, 0.707107f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.z.x, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z.y, -0.707107f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z.z, 0.707107f, 0.001f ) );

		return true;
	}

	TIKI_ADD_TEST( Matrix33CreateRationY )
	{
		Matrix33 test;
		matrix::createRotationY( test, f32::piOver4 );

		TIKI_UT_CHECK( f32::isEquals( test.x.x, 0.707107f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.x.y, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.x.z, -0.707107f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.y.x, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y.y, 1.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y.z, 0.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.z.x, 0.707107f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z.y, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z.z, 0.707107f, 0.001f ) );

		return true;
	}

	TIKI_ADD_TEST( Matrix33CreateRationZ )
	{
		Matrix33 test;
		matrix::createRotationZ( test, f32::piOver4 );

		TIKI_UT_CHECK( f32::isEquals( test.x.x, 0.707107f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.x.y, 0.707107f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.x.z, 0.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.y.x, -0.707107f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y.y, 0.707107f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y.z, 0.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.z.x, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z.y, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z.z, 1.0f, 0.001f ) );

		return true;
	}

	TIKI_ADD_TEST( Matrix33Mul )
	{
		const Matrix33 test1 =
		{
			{ -2.0f, 0.0f, -1.0f },
			{ 5.0f, 0.0f, -4.0f },
			{ -1.0f, 2.0f,  1.0f }
		};

		const Matrix33 test2 =
		{
			{ 3.0f,  6.0f,  4.0f },
			{ 2.0f, -1.0f,  0.0f },
			{ -2.0f,  0.0f, -4.0f }
		};


		Matrix33 test;
		matrix::mul( test, test1, test2 );

		TIKI_UT_CHECK( f32::isEquals( test.x.x, -4.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.x.y, -12.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.x.z, -4.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.y.x, 23.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y.y, 30.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y.z, 36.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.z.x, -1.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z.y, -8.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z.z, -8.0f, 0.001f ) );

		return true;
	}

	TIKI_ADD_TEST( Matrix33Inverse )
	{
		const Matrix33 test =
		{
			{ -2.0f, 0.0f, -1.0f },
			{  5.0f, 0.0f, -4.0f },
			{ -1.0f, 2.0f,  1.0f }
		};

		Matrix33 testInverse;
		TIKI_UT_CHECK( matrix::invert( testInverse, test ) );

		TIKI_UT_CHECK( f32::isEquals( testInverse.x.x, -4.0f / 13.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( testInverse.x.y,  1.0f / 13.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( testInverse.x.z,  0.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( testInverse.y.x, 1.0f / 26.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( testInverse.y.y, 3.0f / 26.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( testInverse.y.z, 1.0f / 2.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( testInverse.z.x, -5.0f / 13.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( testInverse.z.y, -2.0f / 13.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( testInverse.z.z,  0.0f, 0.001f ) );

		return true;
	}

	TIKI_ADD_TEST( Matrix33Transpose )
	{
		const Matrix33 test1 =
		{
			{ -2.0f, 0.0f, -1.0f },
			{ 5.0f, 0.0f, -4.0f },
			{ -1.0f, 2.0f,  1.0f }
		};

		Matrix33 test;
		matrix::transpose( test, test1 );

		TIKI_UT_CHECK( f32::isEquals( test.x.x, -2.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.x.y,  5.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.x.z, -1.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.y.x, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y.y, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y.z, 2.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.z.x, -1.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z.y, -4.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z.z,  1.0f, 0.001f ) );

		return true;
	}

	TIKI_ADD_TEST( Matrix33Transform3 )
	{
		const Matrix33 mtx =
		{
			{ -2.0f, 0.0f, -1.0f },
			{  5.0f, 0.0f, -4.0f },
			{ -1.0f, 2.0f,  1.0f }
		};

		Vector3 test = { 3.0f, 6.0f, 4.0f };
		matrix::transform( test, mtx );

		TIKI_UT_CHECK( f32::isEquals( test.x, -10.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y, -1.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z, 13.0f, 0.001f ) );

		return true;
	}
}