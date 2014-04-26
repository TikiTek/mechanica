
#include "tiki/debuggui/debugguilayout.hpp"

namespace tiki
{
	DebugGuiLayout::DebugGuiLayout()
	{
		m_childControls.create( 32u );
	}

	DebugGuiLayout::~DebugGuiLayout()
	{
		m_childControls.dispose();
	}

	void DebugGuiLayout::addChildControl( DebugGuiControl* pChild )
	{
		m_childControls.push( pChild );
		setRectangle( getRectangle() );
	}

	bool DebugGuiLayout::removeChildControl( DebugGuiControl* pChild )
	{
		const bool found = m_childControls.removeUnsortedByValue( pChild );
		setRectangle( getRectangle() );
		return found;
	}

	void DebugGuiLayout::update()
	{
		for ( uint i = 0u; i < m_childControls.getCount(); ++i )
		{
			m_childControls[ i ]->update();
		}
	}

	void DebugGuiLayout::render( ImmediateRenderer& renderer )
	{
		for ( uint i = 0u; i < m_childControls.getCount(); ++i )
		{
			m_childControls[ i ]->render( renderer );
		}
	}

	bool DebugGuiLayout::processInputEvent( const InputEvent& inputEvent, const DebugGuiInputState& state )
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

	uint DebugGuiLayout::getChildCount() const
	{
		return m_childControls.getCount();
	}

	DebugGuiControl* DebugGuiLayout::getChildByIndex( uint index ) const
	{
		return m_childControls[ index ];
	}
}