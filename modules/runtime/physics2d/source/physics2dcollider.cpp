#include "tiki/physics2d/physics2dcollider.hpp"

#include "tiki/math/vector.hpp"
#include "tiki/physics2d/physics2dshape.hpp"

#include "physics2dinternal.hpp"

namespace tiki
{
	Physics2dCollider::Physics2dCollider()
	{
	}

	Physics2dCollider::~Physics2dCollider()
	{
	}

	void Physics2dCollider::create( const Physics2dShape& shape, const Vector2& position )
	{
	}

	void Physics2dCollider::dispose()
	{
	}

	void* Physics2dCollider::getNativeObject() const
	{
		return m_pBody;
	}
}