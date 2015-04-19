
#include "tiki/unittest/unittest.hpp"

#include "tiki/math/vector.hpp"

namespace tiki
{	
	TIKI_BEGIN_UNITTEST( Vector2 );

	TIKI_ADD_TEST( Vector2Create )
	{
		const Vector2 test1 = { 1.0f, 2.0f };
		const Vector2 test2 = vector::create( 1.0f, 2.0f );

		return test1.x == test2.x && test1.y == test2.y;
	}

	TIKI_ADD_TEST( Vector2Set )
	{
		Vector2 test = { 1.0f, 2.0f };
		vector::set( test, 2.0f, 3.0f );

		return test.x == 2.0f && test.y == 3.0f;
	}

	TIKI_ADD_TEST( Vector2IsEquals )
	{
		const Vector2 test1 = { 1.0f, 2.0f };
		const Vector2 test2 = { 1.1f, 2.1f };

		return vector::isEquals( test1, test2, 0.2f );
	}

	TIKI_ADD_TEST( Vector2IsZero )
	{
		const Vector2 test = { 0.1f, -0.1f };

		return vector::isZero( test, 0.2f );
	}

	TIKI_ADD_TEST( Vector2Add )
	{
		Vector2 test = { 1.0f, 2.0f };
		vector::add( test, 1.0f, -1.0f );

		const Vector2 add = { 3.0f, -3.0f };
		vector::add( test, add );

		return test.x == 5.0f && test.y == -2.0f;
	}

	TIKI_ADD_TEST( Vector2Sub )
	{
		Vector2 test = { 1.0f, 2.0f };

		const Vector2 sub = { 3.0f, -3.0f };
		vector::sub( test, sub );

		return test.x == -2.0f && test.y == 5.0f;
	}

	TIKI_ADD_TEST( Vector2Mul )
	{
		Vector2 test = { 1.0f, 2.0f };

		const Vector2 mul = { 3.0f, -3.0f };
		vector::mul( test, mul );

		return test.x == 3.0f && test.y == -6.0f;
	}

	TIKI_ADD_TEST( Vector2Div )
	{
		Vector2 test = { 6.0f, 9.0f };

		const Vector2 div = { 3.0f, -3.0f };
		vector::div( test, div );

		return test.x == 2.0f && test.y == -3.0f;
	}

	TIKI_ADD_TEST( Vector2Scale )
	{
		Vector2 test = { 1.0f, 2.0f };
		vector::scale( test, 2.0f );

		return test.x == 2.0f && test.y == 4.0f;
	}

	TIKI_ADD_TEST( Vector2Negate )
	{
		Vector2 test = { 1.0f, 2.0f };
		vector::negate( test );

		return test.x == -1.0f && test.y == -2.0f;
	}

	TIKI_ADD_TEST( Vector2Length )
	{
		const Vector2 test = { 1.0f, 2.0f };
		const float legnth = vector::length( test );

		return f32::isEquals( legnth, 2.2360679774997896964091736687313f, 0.01f );
	}

	TIKI_ADD_TEST( Vector2LengthSquared )
	{
		const Vector2 test = { 1.0f, 2.0f };
		const float legnth = vector::lengthSquared( test );

		return f32::isEquals( legnth, 5.0f );
	}
}