
#include "tiki/debuggui/debugguicontrol.hpp"

namespace tiki
{
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
		handleUpdate();

		for ( uint i = 0u; i < m_childControls.getCount(); ++i )
		{
			m_childControls[ i ]->update();
		}
	}

	void DebugGuiControl::render( const ImmediateRenderer& renderer )
	{
		handleRender( renderer );

		for ( uint i = 0u; i < m_childControls.getCount(); ++i )
		{
			m_childControls[ i ]->render( renderer );
		}
	}
}