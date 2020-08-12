#include "tiki/unittest/unittest.hpp"

#include "tiki/math/vector.hpp"

namespace tiki
{	
	TIKI_BEGIN_UNITTEST( Vector2 );

	TIKI_ADD_TEST( Vector2Create )
	{
		const Vector2 test1 = { 1.0f, 2.0f };
		const Vector2 test2 = vector::create( 1.0f, 2.0f );

		TIKI_UT_CHECK( test1.x == test2.x );
		TIKI_UT_CHECK( test1.y == test2.y );
	}

	TIKI_ADD_TEST( Vector2Set )
	{
		Vector2 test = { 1.0f, 2.0f };
		vector::set( test, 2.0f, 3.0f );

		TIKI_UT_CHECK( test.x == 2.0f );
		TIKI_UT_CHECK( test.y == 3.0f );
	}

	TIKI_ADD_TEST( Vector2IsEquals )
	{
		const Vector2 test1 = { 1.0f, 2.0f };
		const Vector2 test2 = { 1.1f, 2.1f };

		TIKI_UT_CHECK( vector::isEquals( test1, test2, 0.2f ) );
	}

	TIKI_ADD_TEST( Vector2IsZero )
	{
		const Vector2 test = { 0.1f, -0.1f };

		TIKI_UT_CHECK( vector::isZero( test, 0.2f ) );
	}

	TIKI_ADD_TEST( Vector2Add )
	{
		Vector2 test1 = { 1.0f, 2.0f };
		Vector2 test2 = { 1.0f, -1.0f };

		Vector2 test;
		vector::add( test, test1, test2 );

		TIKI_UT_CHECK( test.x == 2.0f && test.y == 1.0f );

		const Vector2 add = { 3.0f, -3.0f };
		vector::add( test, add );

		TIKI_UT_CHECK( test.x == 5.0f );
		TIKI_UT_CHECK( test.y == -2.0f );
	}

	TIKI_ADD_TEST( Vector2Sub )
	{
		Vector2 test = { 1.0f, 2.0f };

		const Vector2 sub = { 3.0f, -3.0f };
		vector::sub( test, sub );

		TIKI_UT_CHECK( test.x == -2.0f );
		TIKI_UT_CHECK( test.y == 5.0f );
	}

	TIKI_ADD_TEST( Vector2Mul )
	{
		Vector2 test = { 1.0f, 2.0f };

		const Vector2 mul = { 3.0f, -3.0f };
		vector::mul( test, mul );

		TIKI_UT_CHECK( test.x == 3.0f );
		TIKI_UT_CHECK( test.y == -6.0f );
	}

	TIKI_ADD_TEST( Vector2Div )
	{
		Vector2 test = { 6.0f, 9.0f };

		const Vector2 div = { 3.0f, -3.0f };
		vector::div( test, div );

		TIKI_UT_CHECK( test.x == 2.0f );
		TIKI_UT_CHECK( test.y == -3.0f );
	}

	TIKI_ADD_TEST( Vector2Scale )
	{
		Vector2 test = { 1.0f, 2.0f };
		vector::scale( test, 2.0f );

		TIKI_UT_CHECK( test.x == 2.0f );
		TIKI_UT_CHECK( test.y == 4.0f );
	}

	TIKI_ADD_TEST( Vector2Negate )
	{
		Vector2 test = { 1.0f, 2.0f };
		vector::negate( test );

		TIKI_UT_CHECK( test.x == -1.0f );
		TIKI_UT_CHECK( test.y == -2.0f );
	}

	TIKI_ADD_TEST( Vector2Length )
	{
		const Vector2 test = { 1.0f, 2.0f };
		const float legnth = vector::length( test );

		TIKI_UT_CHECK( f32::isEquals( legnth, 2.2360679774997896964091736687313f, 0.01f ) );
	}

	TIKI_ADD_TEST( Vector2LengthSquared )
	{
		const Vector2 test = { 1.0f, 2.0f };
		const float legnth = vector::lengthSquared( test );

		TIKI_UT_CHECK( f32::isEquals( legnth, 5.0f ) );
	}

	TIKI_ADD_TEST( Vector2Normalize )
	{
		Vector2 test = { 1.0f, 2.0f };
		vector::normalize( test );
				
		TIKI_UT_CHECK( f32::isEquals( test.x, 0.4472135954999579392818347337462f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y, 0.8944271909999158785636694674925f, 0.001f ) );
	}

	TIKI_ADD_TEST( Vector2Truncate )
	{
		Vector2 test = { 1.0f, 2.0f };
		vector::truncate( test, 1.0f );

		TIKI_UT_CHECK( f32::isEquals( test.x, 0.4472135954999579392818347337462f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y, 0.8944271909999158785636694674925f, 0.001f ) );
	}

	TIKI_ADD_TEST( Vector2Distance )
	{
		const Vector2 test1 = { 1.0f, 2.0f };
		const Vector2 test2 = { 4.0f, 3.0f };

		const float distance = vector::distance( test1, test2 );

		TIKI_UT_CHECK( f32::isEquals( distance, 3.1622776601683793319988935444327f, 0.001f ) );
	}

	TIKI_ADD_TEST( Vector2Dot )
	{
		const Vector2 test1 = { 1.0f, 2.0f };
		const Vector2 test2 = { 4.0f, 3.0f };

		const float dot = vector::dot( test1, test2 );

		TIKI_UT_CHECK( f32::isEquals( dot, 10.0f ) );
	}

	TIKI_ADD_TEST( Vector2Reflect )
	{
		const Vector2 testVec = { 0.5f, 0.5f };
		const Vector2 normal = { 0.0f, 1.0f };

		Vector2 test;
		vector::reflect( test, testVec, normal );

		TIKI_UT_CHECK( f32::isEquals( test.x, 0.5f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y, -0.5f ) );
	}

	TIKI_ADD_TEST( Vector2Clamp )
	{
		const Vector2 min = { 0.0f, 0.0f };
		const Vector2 max = { 4.0f, 4.0f };

		Vector2 test = { -2.0f, 7.0f };
		vector::clamp( test, min, max );

		TIKI_UT_CHECK( f32::isEquals( test.x, 0.0f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y, 4.0f ) );
	}

	TIKI_ADD_TEST( Vector2Lerp )
	{
		const Vector2 start = { 0.0f, 0.0f };
		const Vector2 end = { 4.0f, 4.0f };

		Vector2 test;
		vector::lerp( test, start, end, 0.5f );

		TIKI_UT_CHECK( f32::isEquals( test.x, 2.0f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y, 2.0f ) );
	}
}