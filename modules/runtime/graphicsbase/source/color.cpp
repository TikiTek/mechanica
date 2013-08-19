
#include "tiki/graphicsbase/color.hpp"

namespace tiki
{
	Color::Color()
		: r( 0.0f ), g( 0.0f ), b( 0.0f ), a( 0.0f )
	{
	}

	Color::Color( const Color& color )
		: r( color.r), g( color.g ), b( color.b ), a( color.a )
	{

	}

	Color::Color( float r,float g, float b, float a )
		: r( r ), g( g ), b( b ), a( a )
	{
	}

	Color& Color::operator=( const Color& color )
	{
		r = color.r;
		g = color.g;
		b = color.b;
		a = color.a;

		return *this;
	}

	Color Color::transparent	= Color(0.0f,   0.0f,   0.0f,   0.0f);
	Color Color::normalDefault  = Color(0.5f,   0.5f,   1.0f,   1.0f);
	Color Color::white			= Color(1.0f,   1.0f,   1.0f,   1.0f);
	Color Color::black			= Color(0.0f,   0.0f,   0.0f,   1.0f);
	Color Color::red			= Color(1.0f,   0.0f,   0.0f,   1.0f);
	Color Color::pink			= Color(1.0f,	0.5f,	0.76f,	1.0f);
	Color Color::green			= Color(0.0f,   1.0f,   0.0f,   1.0f);
	Color Color::blue			= Color(0.0f,   0.0f,   1.0f,   1.0f);
	Color Color::yellow			= Color(1.0f,   1.0f,   0.0f,   1.0f);
	Color Color::tikiBlue		= Color(0.082f, 0.380f, 0.784f, 1.0f);
	Color Color::cornFlowerBlue = Color(0.39f,  0.58f,  0.93f,  1.0f);
	Color Color::gray			= Color(0.5f,	0.5f,	0.5f,	1.0f);
	Color Color::purple			= Color(0.5f,	0.0f,	0.5f,	1.0f);
	Color Color::orange			= Color(1.0f,	0.64f,	0.0f,	1.0f);
}