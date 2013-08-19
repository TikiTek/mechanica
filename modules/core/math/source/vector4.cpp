
#include "tiki/math/vector4.hpp"

namespace tiki
{
	const Vector4	Vector4::unitW	= Vector4( 0.0f, 0.0f, 0.0f, 1.0f );
	const Vector4	Vector4::unitX	= Vector4( 1.0f, 0.0f, 0.0f, 0.0f );
	const Vector4	Vector4::unitY	= Vector4( 0.0f, 1.0f, 0.0f, 0.0f );
	const Vector4	Vector4::unitZ	= Vector4( 0.0f, 0.0f, 1.0f, 0.0f );
	const Vector4	Vector4::one	= Vector4( 1.0f, 1.0f, 1.0f, 1.0f );
	const Vector4	Vector4::zero	= Vector4( 0.0f, 0.0f, 0.0f, 0.0f );
}