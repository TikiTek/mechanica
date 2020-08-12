#include "tiki/unittest/unittest.hpp"

#include "tiki/math/vector.hpp"

namespace tiki
{
	TIKI_BEGIN_UNITTEST( Vector4 );

	TIKI_ADD_TEST( Vector4Create )
	{
		const Vector4 test1 = { 1.0f, 2.0f, 3.0f, 4.0f };
		const Vector4 test2 = vector::create( 1.0f, 2.0f, 3.0f, 4.0f );

		TIKI_UT_CHECK( test1.x == test2.x );
		TIKI_UT_CHECK( test1.y == test2.y );
		TIKI_UT_CHECK( test1.z == test2.z );
		TIKI_UT_CHECK( test1.w == test2.w );
	}

	TIKI_ADD_TEST( Vector4Set )
	{
		Vector4 test = { 1.0f, 2.0f, 3.0f, 4.0f };
		vector::set( test, 2.0f, 3.0f, 4.0f, 5.0f );

		TIKI_UT_CHECK( test.x == 2.0f );
		TIKI_UT_CHECK( test.y == 3.0f );
		TIKI_UT_CHECK( test.z == 4.0f );
		TIKI_UT_CHECK( test.w == 5.0f );
	}

	TIKI_ADD_TEST( Vector4IsEquals )
	{
		const Vector4 test1 = { 1.0f, 2.0f, 3.0f, 4.0f };
		const Vector4 test2 = { 1.1f, 2.1f, 3.1f, 4.1f };

		TIKI_UT_CHECK( vector::isEquals( test1, test2, 0.2f ) );
	}

	TIKI_ADD_TEST( Vector4IsZero )
	{
		const Vector4 test = { 0.1f, -0.1f, 0.1f, 0.0f };

		TIKI_UT_CHECK( vector::isZero( test, 0.2f ) );
	}

	TIKI_ADD_TEST( Vector4Add )
	{
		Vector4 test1 = { 1.0f, 2.0f, 3.0f, 4.0f };
		Vector4 test2 = { 1.0f, -1.0f, -2.0f, -3.0f };

		Vector4 test;
		vector::add( test, test1, test2 );

		TIKI_UT_CHECK( test.x == 2.0f && test.y == 1.0f  && test.z == 1.0f  && test.w == 1.0f );

		const Vector4 add = { 3.0f, -3.0f, -3.0f, -4.0f };
		vector::add( test, add );

		TIKI_UT_CHECK( test.x == 5.0f );
		TIKI_UT_CHECK( test.y == -2.0f );
		TIKI_UT_CHECK( test.z == -2.0f );
		TIKI_UT_CHECK( test.w == -3.0f );
	}

	TIKI_ADD_TEST( Vector4Sub )
	{
		Vector4 test = { 1.0f, 2.0f, 3.0f, 4.0f };

		const Vector4 sub = { 3.0f, -3.0f, 3.0f, 4.0f };
		vector::sub( test, sub );

		TIKI_UT_CHECK( test.x == -2.0f );
		TIKI_UT_CHECK( test.y == 5.0f );
		TIKI_UT_CHECK( test.z == 0.0f );
		TIKI_UT_CHECK( test.w == 0.0f );
	}

	TIKI_ADD_TEST( Vector4Mul )
	{
		Vector4 test = { 1.0f, 2.0f, 3.0f, 4.0f };

		const Vector4 mul = { 3.0f, -3.0f, 3.0f, 4.0f };
		vector::mul( test, mul );

		TIKI_UT_CHECK( test.x == 3.0f );
		TIKI_UT_CHECK( test.y == -6.0f );
		TIKI_UT_CHECK( test.z == 9.0f );
		TIKI_UT_CHECK( test.w == 16.0f );
	}

	TIKI_ADD_TEST( Vector4Div )
	{
		Vector4 test = { 6.0f, 9.0f, 16.0f, 25.0f };

		const Vector4 div = { 3.0f, -3.0f, 4.0f, 5.0f };
		vector::div( test, div );

		TIKI_UT_CHECK( test.x == 2.0f );
		TIKI_UT_CHECK( test.y == -3.0f );
		TIKI_UT_CHECK( test.z == 4.0f );
		TIKI_UT_CHECK( test.w == 5.0f );
	}

	TIKI_ADD_TEST( Vector4Scale )
	{
		Vector4 test = { 1.0f, 2.0f, 3.0f, 4.0f };
		vector::scale( test, 2.0f );

		TIKI_UT_CHECK( test.x == 2.0f );
		TIKI_UT_CHECK( test.y == 4.0f );
		TIKI_UT_CHECK( test.z == 6.0f );
		TIKI_UT_CHECK( test.w == 8.0f );
	}

	TIKI_ADD_TEST( Vector4Negate )
	{
		Vector4 test = { 1.0f, 2.0f, 3.0f, 4.0f };
		vector::negate( test );

		TIKI_UT_CHECK( test.x == -1.0f );
		TIKI_UT_CHECK( test.y == -2.0f );
		TIKI_UT_CHECK( test.z == -3.0f );
		TIKI_UT_CHECK( test.w == -4.0f );
	}

	TIKI_ADD_TEST( Vector4Length )
	{
		const Vector4 test = { 1.0f, 2.0f, 3.0f, 4.0f };
		const float legnth = vector::length( test );
		
		TIKI_UT_CHECK( f32::isEquals( legnth, 5.477225575051661134569697828008f, 0.01f ) );
	}

	TIKI_ADD_TEST( Vector4LengthSquared )
	{
		const Vector4 test = { 1.0f, 2.0f, 3.0f, 4.0f };
		const float legnth = vector::lengthSquared( test );
		
		TIKI_UT_CHECK( f32::isEquals( legnth, 30.0f ) );
	}

	TIKI_ADD_TEST( Vector4Normalize )
	{
		Vector4 test = { 1.0f, 2.0f, 3.0f, 4.0f };
		vector::normalize( test );

		TIKI_UT_CHECK( f32::isEquals( test.x, 0.1825741858350553711523232609336f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y, 0.3651483716701107423046465218672f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z, 0.5477225575051661134569697828008f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.w, 0.7302967433402214846092930437344f, 0.001f ) );
	}

	TIKI_ADD_TEST( Vector4Truncate )
	{
		Vector4 test = { 1.0f, 2.0f, 3.0f, 4.0f };
		vector::truncate( test, 1.0f );

		TIKI_UT_CHECK( f32::isEquals( test.x, 0.1825741858350553711523232609336f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y, 0.3651483716701107423046465218672f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z, 0.5477225575051661134569697828008f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.w, 0.7302967433402214846092930437344f, 0.001f ) );
	}

	TIKI_ADD_TEST( Vector4Distance )
	{
		const Vector4 test1 = { 1.0f, 2.0f, 3.0f, 4.0f };
		const Vector4 test2 = { 4.0f, 3.0f, 2.0f, 1.0f };

		const float distance = vector::distance( test1, test2 );

		TIKI_UT_CHECK( f32::isEquals( distance, 4.4721359549995793928183473374626f, 0.001f ) );
	}

	TIKI_ADD_TEST( Vector4Dot )
	{
		const Vector4 test1 = { 1.0f, 2.0f, 3.0f, 4.0f };
		const Vector4 test2 = { 4.0f, 3.0f, 2.0f, 1.0f };

		const float dot = vector::dot( test1, test2 );

		TIKI_UT_CHECK( f32::isEquals( dot, 20.0f ) );
	}

	TIKI_ADD_TEST( Vector4Reflect )
	{
		const Vector4 testVec = { 0.5f, 0.5f, 0.5f, 0.5f };
		const Vector4 normal = { 0.0f, 1.0f, 0.0f, 0.0f };

		Vector4 test;
		vector::reflect( test, testVec, normal );

		TIKI_UT_CHECK( f32::isEquals( test.x, 0.5f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y, -0.5f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z, 0.5f ) );
		TIKI_UT_CHECK( f32::isEquals( test.w, 0.5f ) );
	}

	TIKI_ADD_TEST( Vector4Clamp )
	{
		const Vector4 min = { 0.0f, 0.0f, 0.0f, 0.0f };
		const Vector4 max = { 4.0f, 4.0f, 4.0f, 4.0f };

		Vector4 test = { -2.0f, 1.0f, 4.0f, 7.0f };
		vector::clamp( test, min, max );

		TIKI_UT_CHECK( f32::isEquals( test.x, 0.0f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y, 1.0f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z, 4.0f ) );
		TIKI_UT_CHECK( f32::isEquals( test.w, 4.0f ) );
	}

	TIKI_ADD_TEST( Vector4Lerp )
	{
		const Vector4 start = { 0.0f, 0.0f, 0.0f, 0.0f };
		const Vector4 end = { 4.0f, 4.0f, 4.0f, 4.0f };

		Vector4 test;
		vector::lerp( test, start, end, 0.5f );

		TIKI_UT_CHECK( f32::isEquals( test.x, 2.0f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y, 2.0f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z, 2.0f ) );
		TIKI_UT_CHECK( f32::isEquals( test.w, 2.0f ) );
	}
}