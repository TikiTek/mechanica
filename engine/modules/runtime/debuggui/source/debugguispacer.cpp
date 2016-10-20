
#include "tiki/debuggui/debugguispacer.hpp"

namespace tiki
{
	void DebugGuiSpacer::create( const Vector2& minSize )
	{
		m_minSize = minSize;
	}

	void DebugGuiSpacer::dispose()
	{
		vector::clear( m_minSize );
	}

	Vector2 DebugGuiSpacer::getMinimumSize()
	{
		return m_minSize;
	}

	void DebugGuiSpacer::update( double /*elapsedTime*/ )
	{
	}

	void DebugGuiSpacer::render( ImmediateRenderer& /*renderer*/ )
	{
	}

	bool DebugGuiSpacer::processInputEvent( const InputEvent& /*inputEvent*/, const DebugGuiInputState& /*state*/ )
	{
		return false;
	}

	void DebugGuiSpacer::handleRectangleChanged( const AxisAlignedRectangle& /*boundingRectangle*/ )
	{
	}
}
