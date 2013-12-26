
#include "tiki/math/quaternion.hpp"

namespace tiki
{
	const Quaternion Quaternion::zero		= { 0.0f, 0.0f, 0.0f, 0.0f };
	const Quaternion Quaternion::one		= { 1.0f, 1.0f, 1.0f, 1.0f };
	const Quaternion Quaternion::identity	= { 0.0f, 0.0f, 0.0f, 1.0f };
}