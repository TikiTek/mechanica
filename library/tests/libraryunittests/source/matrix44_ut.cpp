#include "tiki/unittest/unittest.hpp"

#include "tiki/math/matrix.hpp"

namespace tiki
{
	TIKI_BEGIN_UNITTEST( Matrix44 );

	TIKI_ADD_TEST( Matrix44Create )
	{
		const Vector3 test1 = { 1.0f, 2.0f, 3.0f };
		const Vector3 test2 = vector::create( 1.0f, 2.0f, 3.0f );

		TIKI_UT_CHECK( test1.x == test2.x );
		TIKI_UT_CHECK( test1.y == test2.y );
		TIKI_UT_CHECK( test1.z == test2.z );

		return true;
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
		matrix::invert( testInverse, test );

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

		return true;
	}
}