#include "tiki/unittest/unittest.hpp"

#include "tiki/math/matrix.hpp"

namespace tiki
{
	TIKI_BEGIN_UNITTEST( Matrix44 );

	TIKI_ADD_TEST( Matrix44CreateScale )
	{
		const Vector3 scale = { 5.0f, 5.0f, 5.0f };

		Matrix44 test;
		matrix::createScale( test, scale );

		TIKI_UT_CHECK( f32::isEquals( test.x.x, 5.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.x.y, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.x.z, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.x.w, 0.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.y.x, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y.y, 5.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y.z, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y.w, 0.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.z.x, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z.y, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z.z, 5.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z.w, 0.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.w.x, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.w.y, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.w.z, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.w.w, 1.0f, 0.001f ) );
	}

	TIKI_ADD_TEST( Matrix44CreateRationX )
	{
		Matrix44 test;
		matrix::createRotationX( test, f32::piOver4 );

		TIKI_UT_CHECK( f32::isEquals( test.x.x, 1.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.x.y, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.x.z, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.x.w, 0.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.y.x, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y.y, 0.707107f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y.z, 0.707107f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y.w, 0.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.z.x, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z.y, -0.707107f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z.z, 0.707107f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z.w, 0.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.w.x, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.w.y, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.w.z, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.w.w, 1.0f, 0.001f ) );
	}

	TIKI_ADD_TEST( Matrix44CreateRationY )
	{
		Matrix44 test;
		matrix::createRotationY( test, f32::piOver4 );

		TIKI_UT_CHECK( f32::isEquals( test.x.x, 0.707107f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.x.y, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.x.z, -0.707107f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.x.w, 0.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.y.x, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y.y, 1.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y.z, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y.w, 0.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.z.x, 0.707107f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z.y, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z.z, 0.707107f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z.w, 0.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.w.x, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.w.y, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.w.z, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.w.w, 1.0f, 0.001f ) );
	}

	TIKI_ADD_TEST( Matrix44CreateRationZ )
	{
		Matrix44 test;
		matrix::createRotationZ( test, f32::piOver4 );

		TIKI_UT_CHECK( f32::isEquals( test.x.x, 0.707107f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.x.y, 0.707107f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.x.z, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.x.w, 0.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.y.x, -0.707107f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y.y, 0.707107f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y.z, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y.w, 0.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.z.x, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z.y, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z.z, 1.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z.w, 0.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.w.x, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.w.y, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.w.z, 0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.w.w, 1.0f, 0.001f ) );
	}

	TIKI_ADD_TEST( Matrix44Mul )
	{
		const Matrix44 test1 =
		{
			{ -2.0f, 0.0f, -1.0f,  1.0f },
			{  5.0f, 0.0f, -4.0f, -3.0f },
			{ -1.0f, 2.0f,  1.0f, -1.0f },
			{  5.0f, 1.0f, -3.0f,  1.0f }			
		};

		const Matrix44 test2 =
		{
			{  3.0f,  6.0f,  4.0f,  2.0f },
			{  2.0f, -1.0f,  0.0f,  2.0f },
			{ -2.0f,  0.0f, -4.0f, -1.0f },
			{ -3.0f, -5.0f, -1.0f, -1.0f }
		};


		Matrix44 test;
		matrix::mul( test, test1, test2 );

		TIKI_UT_CHECK( f32::isEquals( test.x.x, -7.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.x.y, -17.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.x.z, -5.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.x.w, -4.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.y.x, 32.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y.y, 45.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y.z, 39.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y.w, 17.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.z.x, 2.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z.y, -3.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z.z, -7.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z.w, 2.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.w.x, 20.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.w.y, 24.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.w.z, 31.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.w.w, 14.0f, 0.001f ) );
	}

	TIKI_ADD_TEST( Matrix44Inverse )
	{
		Matrix44 test = {
			{ -6.0f,  1.0f,  1.0f, -5.0f },
			{ -2.0f,  3.0f, -1.0f,  0.0f },
			{ -2.0f, -5.0f, -5.0f, -3.0f },
			{  8.0f,  7.0f, -4.0f,  0.0f }
		};

		Matrix44 testInverse;
		TIKI_UT_CHECK( matrix::invert( testInverse, test ) );

		TIKI_UT_CHECK( f32::isEquals( testInverse.x.x,  0.010190217391f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( testInverse.x.y, -0.209239130435f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( testInverse.x.z, -0.016983695652f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( testInverse.x.w,  0.076086956522f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( testInverse.y.x,  0.032608695652f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( testInverse.y.y,  0.130434782609f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( testInverse.y.z, -0.054347826087f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( testInverse.y.w,  0.043478260870f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( testInverse.z.x,  0.077445652174f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( testInverse.z.y, -0.190217391304f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( testInverse.z.z, -0.129076086957f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( testInverse.z.w, -0.021739130435f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( testInverse.w.x, -0.190217391304f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( testInverse.w.y,  0.239130434783f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( testInverse.w.z, -0.016304347826f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( testInverse.w.w, -0.086956521739f, 0.001f ) );
	}

	TIKI_ADD_TEST( Matrix44Transpose )
	{
		const Matrix44 test1 =
		{
			{ -2.0f, 0.0f, -1.0f,  1.0f },
			{  5.0f, 0.0f, -4.0f, -3.0f },
			{ -1.0f, 2.0f,  1.0f, -1.0f },
			{  5.0f, 1.0f, -3.0f,  1.0f }
		};

		Matrix44 test;
		matrix::transpose( test, test1 );

		TIKI_UT_CHECK( f32::isEquals( test.x.x, -2.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.x.y,  5.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.x.z, -1.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.x.w,  5.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.y.x,  0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y.y,  0.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y.z,  2.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y.w,  1.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.z.x, -1.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z.y, -4.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z.z,  1.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z.w, -3.0f, 0.001f ) );

		TIKI_UT_CHECK( f32::isEquals( test.w.x,  1.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.w.y, -3.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.w.z, -1.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.w.w,  1.0f, 0.001f ) );
	}

	TIKI_ADD_TEST( Matrix44Transform3 )
	{
		const Matrix44 mtx =
		{
			{ -2.0f, 0.0f, -1.0f,  1.0f },
			{ 5.0f, 0.0f, -4.0f, -3.0f },
			{ -1.0f, 2.0f,  1.0f, -1.0f },
			{ 5.0f, 1.0f, -3.0f,  1.0f }
		};

		Vector3 test = { 3.0f, 6.0f, 4.0f };
		matrix::transform( test, mtx );

		TIKI_UT_CHECK( f32::isEquals( test.x, -0.9f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y, -0.4f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z,  1.2f, 0.001f ) );
	}

	TIKI_ADD_TEST( Matrix44Transform4 )
	{
		const Matrix44 mtx =
		{
			{ -2.0f, 0.0f, -1.0f,  1.0f },
			{ 5.0f, 0.0f, -4.0f, -3.0f },
			{ -1.0f, 2.0f,  1.0f, -1.0f },
			{ 5.0f, 1.0f, -3.0f,  1.0f }
		};

		Vector4 test = { 3.0f, 6.0f, 4.0f, 2.0f };
		matrix::transform( test, mtx );

		TIKI_UT_CHECK( f32::isEquals( test.x, -8.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y, -7.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z, 11.0f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.w, 11.0f, 0.001f ) );
	}
}