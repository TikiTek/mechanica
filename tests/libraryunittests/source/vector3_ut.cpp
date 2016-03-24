#include "tiki/unittest/unittest.hpp"

#include "tiki/math/vector.hpp"

namespace tiki
{
	TIKI_BEGIN_UNITTEST( Vector3 );

	TIKI_ADD_TEST( Vector3Create )
	{
		const Vector3 test1 = { 1.0f, 2.0f, 3.0f };
		const Vector3 test2 = vector::create( 1.0f, 2.0f, 3.0f );

		TIKI_UT_CHECK( test1.x == test2.x );
		TIKI_UT_CHECK( test1.y == test2.y );
		TIKI_UT_CHECK( test1.z == test2.z );

		return true;
	}

	TIKI_ADD_TEST( Vector3Set )
	{
		Vector3 test = { 1.0f, 2.0f, 3.0f };
		vector::set( test, 2.0f, 3.0f, 4.0f );

		TIKI_UT_CHECK( test.x == 2.0f );
		TIKI_UT_CHECK( test.y == 3.0f );
		TIKI_UT_CHECK( test.z == 4.0f );

		return true;
	}

	TIKI_ADD_TEST( Vector3IsEquals )
	{
		const Vector3 test1 = { 1.0f, 2.0f, 3.0f };
		const Vector3 test2 = { 1.1f, 2.1f, 3.1f };

		return vector::isEquals( test1, test2, 0.2f );
	}

	TIKI_ADD_TEST( Vector3IsZero )
	{
		const Vector3 test = { 0.1f, -0.1f, 0.1f };

		return vector::isZero( test, 0.2f );
	}

	TIKI_ADD_TEST( Vector3Add )
	{
		Vector3 test1 = { 1.0f, 2.0f, 3.0f };
		Vector3 test2 = { 1.0f, -1.0f, -2.0f };

		Vector3 test;
		vector::add( test, test1, test2 );

		TIKI_UT_CHECK( test.x == 2.0f && test.y == 1.0f && test.z == 1.0f );

		const Vector3 add = { 3.0f, -3.0f, -3.0f };
		vector::add( test, add );

		TIKI_UT_CHECK( test.x == 5.0f );
		TIKI_UT_CHECK( test.y == -2.0f );
		TIKI_UT_CHECK( test.z == -2.0f );

		return true;
	}

	TIKI_ADD_TEST( Vector3Sub )
	{
		Vector3 test = { 1.0f, 2.0f, 3.0f };

		const Vector3 sub = { 3.0f, -3.0f, 3.0f };
		vector::sub( test, sub );

		TIKI_UT_CHECK( test.x == -2.0f );
		TIKI_UT_CHECK( test.y == 5.0f );
		TIKI_UT_CHECK( test.z == 0.0f );

		return true;
	}

	TIKI_ADD_TEST( Vector3Mul )
	{
		Vector3 test = { 1.0f, 2.0f, 3.0f };

		const Vector3 mul = { 3.0f, -3.0f, 3.0f };
		vector::mul( test, mul );

		TIKI_UT_CHECK( test.x == 3.0f );
		TIKI_UT_CHECK( test.y == -6.0f );
		TIKI_UT_CHECK( test.z == 9.0f );

		return true;
	}

	TIKI_ADD_TEST( Vector3Div )
	{
		Vector3 test = { 6.0f, 9.0f, 16.0f };

		const Vector3 div = { 3.0f, -3.0f, 4.0f };
		vector::div( test, div );

		TIKI_UT_CHECK( test.x == 2.0f );
		TIKI_UT_CHECK( test.y == -3.0f );
		TIKI_UT_CHECK( test.z == 4.0f );

		return true;
	}

	TIKI_ADD_TEST( Vector3Scale )
	{
		Vector3 test = { 1.0f, 2.0f, 3.0f };
		vector::scale( test, 2.0f );

		TIKI_UT_CHECK( test.x == 2.0f );
		TIKI_UT_CHECK( test.y == 4.0f );
		TIKI_UT_CHECK( test.z == 6.0f );

		return true;
	}

	TIKI_ADD_TEST( Vector3Negate )
	{
		Vector3 test = { 1.0f, 2.0f, 3.0f };
		vector::negate( test );

		TIKI_UT_CHECK( test.x == -1.0f );
		TIKI_UT_CHECK( test.y == -2.0f );
		TIKI_UT_CHECK( test.z == -3.0f );

		return true;
	}

	TIKI_ADD_TEST( Vector3Length )
	{
		const Vector3 test = { 1.0f, 2.0f, 3.0f };
		const float legnth = vector::length( test );

		return f32::isEquals( legnth, 3.7416573867739413f, 0.01f );
	}

	TIKI_ADD_TEST( Vector3LengthSquared )
	{
		const Vector3 test = { 1.0f, 2.0f, 3.0f };
		const float legnth = vector::lengthSquared( test );

		return f32::isEquals( legnth, 14.0f );
	}

	TIKI_ADD_TEST( Vector3Normalize )
	{
		Vector3 test = { 1.0f, 2.0f, 3.0f };
		vector::normalize( test );

		TIKI_UT_CHECK( f32::isEquals( test.x, 0.267261242f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y, 0.534522484f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z, 0.801783726f, 0.001f ) );

		return true;
	}

	TIKI_ADD_TEST( Vector3Truncate )
	{
		Vector3 test = { 1.0f, 2.0f, 3.0f };
		vector::truncate( test, 1.0f );

		TIKI_UT_CHECK( f32::isEquals( test.x, 0.267261242f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y, 0.534522484f, 0.001f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z, 0.801783726f, 0.001f ) );

		return true;
	}

	TIKI_ADD_TEST( Vector3Distance )
	{
		const Vector3 test1 = { 1.0f, 2.0f, 3.0f };
		const Vector3 test2 = { 4.0f, 3.0f, 2.0f };

		const float distance = vector::distance( test1, test2 );

		TIKI_UT_CHECK( f32::isEquals( distance, 3.3166247903554f, 0.001f ) );

		return true;
	}

	TIKI_ADD_TEST( Vector3Dot )
	{
		const Vector3 test1 = { 1.0f, 2.0f, 3.0f };
		const Vector3 test2 = { 4.0f, 3.0f, 2.0f };

		const float dot = vector::dot( test1, test2 );

		TIKI_UT_CHECK( f32::isEquals( dot, 16.0f ) );

		return true;
	}

	TIKI_ADD_TEST( Vector3Reflect )
	{
		const Vector3 testVec = { 0.5f, 0.5f, 0.5f };
		const Vector3 normal = { 0.0f, 1.0f, 0.0f };

		Vector3 test;
		vector::reflect( test, testVec, normal );

		TIKI_UT_CHECK( f32::isEquals( test.x, 0.5f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y, -0.5f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z, 0.5f ) );

		return true;
	}

	TIKI_ADD_TEST( Vector3Clamp )
	{
		const Vector3 min = { 0.0f, 0.0f, 0.0f };
		const Vector3 max = { 4.0f, 4.0f, 4.0f };

		Vector3 test = { -2.0f, 1.0f, 7.0f };
		vector::clamp( test, min, max );

		TIKI_UT_CHECK( f32::isEquals( test.x, 0.0f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y, 1.0f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z, 4.0f ) );

		return true;
	}

	TIKI_ADD_TEST( Vector3Lerp )
	{
		const Vector3 start = { 0.0f, 0.0f, 0.0f };
		const Vector3 end = { 4.0f, 4.0f, 4.0f };

		Vector3 test;
		vector::lerp( test, start, end, 0.5f );

		TIKI_UT_CHECK( f32::isEquals( test.x, 2.0f ) );
		TIKI_UT_CHECK( f32::isEquals( test.y, 2.0f ) );
		TIKI_UT_CHECK( f32::isEquals( test.z, 2.0f ) );

		return true;
	}
}