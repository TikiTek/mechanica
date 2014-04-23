
#include "tiki/debuggui/debugguicontrol.hpp"

namespace tiki
{
	const Font* DebugGuiControl::s_pDefaultFont = nullptr;

	void DebugGuiControl::addChildControl( DebugGuiControl* pChild )
	{
		m_childControls.push( pChild );
	}

	bool DebugGuiControl::removeChildControl( DebugGuiControl* pChild )
	{
		return m_childControls.removeUnsortedByValue( pChild );
	}

	void DebugGuiControl::update()
	{
		for ( uint i = 0u; i < m_childControls.getCount(); ++i )
		{
			m_childControls[ i ]->update();
		}
	}

	void DebugGuiControl::render( ImmediateRenderer& renderer )
	{
		for ( uint i = 0u; i < m_childControls.getCount(); ++i )
		{
			m_childControls[ i ]->render( renderer );
		}
	}

	bool DebugGuiControl::processInputEvent( const InputEvent& inputEvent, const DebugGuiInputState& state )
	{
		for ( uint i = 0u; i < m_childControls.getCount(); ++i )
		{
			if ( m_childControls[ i ]->processInputEvent( inputEvent, state ) )
			{
				return true;
			}
		}

		return false;
	}
}