
#include "tiki/debuggui/debugguialignment.hpp"

#include "tiki/base/functions.hpp"

namespace tiki
{
	void DebugGuiAlignment::create( OrientationFlags flags, const Vector2& alignment )
	{
		m_flags		= flags;
		m_alignment	= alignment;
	}

	void DebugGuiAlignment::dispose()
	{
		m_flags		= OrientationFlags_Node;
		vector::clear( m_alignment );
	}

	Vector2 DebugGuiAlignment::getMinimumSize()
	{
		Vector2 minSize = { 0.0f, 0.0f };

		if ( isBitSet(m_flags, OrientationFlags_X ) )
		{
			const float modAlign = f32::mod( getRectangle().getLeft(), m_alignment.x );
			minSize.x = m_alignment.x - modAlign;
		}

		if ( isBitSet(m_flags, OrientationFlags_Y ) )
		{
			const float modAlign = f32::mod( getRectangle().getTop(), m_alignment.y );
			minSize.y = m_alignment.y - modAlign;
		}
		
		return minSize;
	}

	void DebugGuiAlignment::update( double /*elapsedTime*/ )
	{
	}

	void DebugGuiAlignment::render( ImmediateRenderer& /*renderer*/ )
	{
	}

	bool DebugGuiAlignment::processInputEvent( const InputEvent& /*inputEvent*/, const DebugGuiInputState& /*state*/ )
	{
		return false;
	}

	void DebugGuiAlignment::handleRectangleChanged( const AxisAlignedRectangle& /*boundingRectangle*/ )
	{
	}
}
