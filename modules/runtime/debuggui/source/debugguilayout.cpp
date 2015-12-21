
#include "tiki/debuggui/debugguilayout.hpp"

namespace tiki
{
	DebugGuiLayout::DebugGuiLayout()
	{
	}

	DebugGuiLayout::~DebugGuiLayout()
	{
	}

	void DebugGuiLayout::create()
	{
		TIKI_ASSERT( m_childControls.getCount() == 0u );
	}

	void DebugGuiLayout::dispose()
	{
		for ( DebugGuiControl& child : m_childControls )
		{
			child.setParent(nullptr);
		}
		m_childControls.clear();
	}

	void DebugGuiLayout::addChildControl( DebugGuiControl* pChild )
	{
		TIKI_ASSERT( pChild != nullptr );
		TIKI_ASSERT( pChild->getParent() == nullptr );

		m_childControls.push( pChild );
		pChild->setParent( this );

		refreshRectangle();
	}

	void DebugGuiLayout::removeChildControl( DebugGuiControl* pChild )
	{
		TIKI_ASSERT( pChild != nullptr );
		TIKI_ASSERT( pChild->getParent() == this );

		m_childControls.removeSortedByValue( *pChild );
		pChild->setParent( nullptr );

		refreshRectangle();
	}

	void DebugGuiLayout::update( double elapsedTime )
	{
		for ( LinkedIterator< DebugGuiControl > it = m_childControls.getBegin(); it != m_childControls.getEnd(); ++it )
		{
			it->update( elapsedTime );
		}
	}

	void DebugGuiLayout::render( ImmediateRenderer& renderer )
	{
		for ( LinkedIterator< DebugGuiControl > it = m_childControls.getBegin(); it != m_childControls.getEnd(); ++it )
		{
			it->render( renderer );
		}
	}

	bool DebugGuiLayout::processInputEvent( const InputEvent& inputEvent, const DebugGuiInputState& state )
	{
		for ( LinkedIterator< DebugGuiControl > it = m_childControls.getBegin(); it != m_childControls.getEnd(); ++it )
		{
			if ( it->processInputEvent( inputEvent, state ) )
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

	LinkedIterator< DebugGuiControl > DebugGuiLayout::getChildrenBegin()
	{
		return m_childControls.getBegin();
	}

	LinkedIterator< DebugGuiControl > DebugGuiLayout::getChildrenEnd()
	{
		return m_childControls.getEnd();
	}
}