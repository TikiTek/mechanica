
#include "tiki/math/vector.hpp"

namespace tiki
{
	const Vector2 Vector2::zero		= { 0.0f, 0.0f };
	const Vector2 Vector2::one		= { 1.0f, 1.0f };
	const Vector2 Vector2::unitX	= { 1.0f, 0.0f };
	const Vector2 Vector2::unitY	= { 0.0f, 1.0f };

	const Vector3 Vector3::zero		= {  0.0f,  0.0f,  0.0f };
	const Vector3 Vector3::one		= {  1.0f,  1.0f,  1.0f };
	const Vector3 Vector3::unitX	= {  1.0f,  0.0f,  0.0f };
	const Vector3 Vector3::unitY	= {  0.0f,  1.0f,  0.0f };
	const Vector3 Vector3::unitZ	= {  0.0f,  0.0f,  1.0f };
	const Vector3 Vector3::forward  = {  0.0f,  0.0f, -1.0f };
	const Vector3 Vector3::down		= {  0.0f, -1.0f,  0.0f };
	const Vector3 Vector3::left		= { -1.0f,  0.0f,  0.0f };

	const Vector4 Vector4::zero		= { 0.0f, 0.0f, 0.0f, 0.0f };
	const Vector4 Vector4::one		= { 1.0f, 1.0f, 1.0f, 1.0f };
	const Vector4 Vector4::unitX	= { 1.0f, 0.0f, 0.0f, 0.0f };
	const Vector4 Vector4::unitY	= { 0.0f, 1.0f, 0.0f, 0.0f };
	const Vector4 Vector4::unitZ	= { 0.0f, 0.0f, 1.0f, 0.0f };
	const Vector4 Vector4::unitW	= { 0.0f, 0.0f, 0.0f, 1.0f };
}