
#include "tiki/debugguiwindows/debugguiwindow_debugprop.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/base/debugprop.hpp"
#include "tiki/base/debugpropmanager.hpp"
#include "tiki/base/string.hpp"
#include "tiki/debuggui/debuggui.hpp"
#include "tiki/debuggui/debugguievent.hpp"
#include "tiki/graphics/color.hpp"
#include "tiki/graphics/immediaterenderer.hpp"
#include "tiki/input/inputevent.hpp"

namespace tiki
{
	TIKI_DEBUGPROP_BOOL( s_test1, "test/bbb", false );
	TIKI_DEBUGPROP_BOOL( s_test2, "test/ccc", false );
	TIKI_DEBUGPROP_BOOL( s_test3, "test2/bbb", false );
	TIKI_DEBUGPROP_BOOL( s_test4, "test3/bbb", false );
	TIKI_DEBUGPROP_INT( s_test5, "test2/ccc", 1, 0, 10 );
	TIKI_DEBUGPROP_INT( s_test6, "test3/ccc", 5, 4, 50 );
	TIKI_DEBUGPROP_FLOAT( s_test7, "test2/ddd", 1.0f, 0.0f, 10.0f );
	TIKI_DEBUGPROP_FLOAT( s_test8, "test3/ddd", 5.0f, 4.0f, 50.0f );

	void DebugGuiWindowDebugProp::create( DebugGui& debugGui )
	{
		m_selectedProp = TIKI_SIZE_T_MAX;

		m_baseLayout.create();
		m_baseLayout.setExpandChildren( false );

		DebugGuiWindow::create( debugGui, "Debug Properties", m_baseLayout );

		LinkedList< DebugProp >& debugProperties = debugprop::getProperties();

		SizedArray<string> folderNames;
		folderNames.create( debugProperties.getCount() * 5u );

		for (DebugProp& prop : debugProperties)
		{
			string pathName;
			pathName += prop.getModule();
			pathName += "/";
			pathName += prop.getFullName();
			pathName = pathName.toLower();

			int currentIndex = 0;
			while ( currentIndex < pathName.getLength() )
			{
				int nextIndex = pathName.indexOf( '/', currentIndex );
				if ( nextIndex == -1 )
				{
					break;
				}

				const string folderName = pathName.subString( 0, nextIndex );
				if ( folderNames.getIndexOf( folderName ) == TIKI_SIZE_T_MAX )
				{
					folderNames.push( folderName );
				}

				currentIndex = nextIndex + 1;
			}

		}

		m_folderNodes.create( folderNames.getCount() );
		m_propNodes.create( debugProperties.getCount() );

		for (uint folderIndex = 0u; folderIndex < m_folderNodes.getCount(); ++folderIndex)
		{
			const string& name = folderNames[ folderIndex ];
			TreeFolderNode& node = m_folderNodes[ folderIndex ];

			int lastIndex = name.lastIndexOf( '/' ) + 1;
			const string nodeName = name.subString( lastIndex, name.getLength() - lastIndex );

			node.fullLayout.create();
			node.nodeLayout.create();
			node.spaceLayout.create();
			node.chilrenLayout.create();

			setLayoutParameters( node.fullLayout );
			setLayoutParameters( node.nodeLayout );
			setLayoutParameters( node.spaceLayout );
			setLayoutParameters( node.chilrenLayout );

			node.expandButton.create( "+" );
			node.nameLabel.create( nodeName.cStr() );

			node.nodeLayout.addChildControl( &node.expandButton );
			node.nodeLayout.addChildControl( &node.nameLabel );

			node.spacer.create( vector::create( 25.0f, 0.0f ) );

			node.spaceLayout.addChildControl( &node.spacer );
			node.spaceLayout.addChildControl( &node.chilrenLayout );

			node.fullLayout.addChildControl( &node.nodeLayout );
			node.fullLayout.addChildControl( &node.spaceLayout );

			node.parentIndex = TIKI_SIZE_T_MAX;
			if ( lastIndex == 0 )
			{
				m_baseLayout.addChildControl( &node.fullLayout );
			}
			else
			{
				const string parentName = name.subString( 0, lastIndex - 1 );
				for (uint parentIndex = 0u; parentIndex < folderIndex; ++parentIndex)
				{
					if ( folderNames[ parentIndex ] == parentName )
					{
						node.parentIndex = parentIndex;
					}
				}
				TIKI_ASSERT( node.parentIndex != TIKI_SIZE_T_MAX );
			}
		}

		uint propIndex = 0;
		for (DebugProp& prop : debugProperties)
		{
			TreePropNode& node = m_propNodes[ propIndex ];

			node.nodeLayout.create();

			setLayoutParameters( node.nodeLayout );

			node.nameLabel.create( prop.getName() );
			node.valueAlignment.create( DebugGuiAlignment::OrientationFlags_X, vector::create( 400.0f, 0.0f ) );
			setDebugPropText( node.valueLabel, prop );

			node.nodeLayout.addChildControl( &node.nameLabel );
			node.nodeLayout.addChildControl( &node.valueAlignment );
			node.nodeLayout.addChildControl( &node.valueLabel );

			string pathName;
			pathName += prop.getModule();
			pathName += "/";
			pathName += prop.getFullName();
			pathName = pathName.toLower();
			pathName = pathName.subString( 0, pathName.lastIndexOf( '/' ) );

			node.parentIndex = TIKI_SIZE_T_MAX;
			for (uint parentIndex = 0u; parentIndex < m_folderNodes.getCount(); ++parentIndex)
			{
				if (folderNames[ parentIndex ] == pathName )
				{
					node.parentIndex = parentIndex;
				}
			}
			TIKI_ASSERT( node.parentIndex != TIKI_SIZE_T_MAX );

			propIndex++;
		}

		folderNames.dispose();

		setRectangle( Rectangle( 20.0, 40.0f, 200.0f, 400.0f ) );
	}

	void DebugGuiWindowDebugProp::dispose()
	{
		m_baseLayout.dispose();

		for (uint i = 0u; i < m_folderNodes.getCount(); ++i)
		{
			TreeFolderNode& node = m_folderNodes[ i ];

			node.nodeLayout.dispose();
			node.fullLayout.dispose();
			node.spaceLayout.dispose();
			node.chilrenLayout.dispose();

			node.spacer.dispose();
			node.expandButton.dispose();
			node.nameLabel.dispose();
		}

		for (uint i = 0u; i < m_propNodes.getCount(); ++i)
		{
			TreePropNode& node = m_propNodes[ i ];

			node.nameLabel.dispose();
			node.valueLabel.dispose();

			node.nodeLayout.dispose();
		}

		m_folderNodes.dispose();
		m_propNodes.dispose();

		DebugGuiWindow::dispose();
	}

	void DebugGuiWindowDebugProp::render( ImmediateRenderer& renderer )
	{
		if ( m_selectedProp != TIKI_SIZE_T_MAX )
		{
			TreePropNode& selectedNode = m_propNodes[ m_selectedProp ];
			renderer.drawRectangle( selectedNode.nodeLayout.getRectangle(), TIKI_COLOR( 194, 194, 255, 164 ) );
		}

		DebugGuiWindow::render( renderer );
	}

	bool DebugGuiWindowDebugProp::processInputEvent( const InputEvent& inputEvent, const DebugGuiInputState& state )
	{
		if ( inputEvent.eventType == InputEventType_Mouse_ButtonDown && inputEvent.data.mouseButton.button == MouseButton_Left )
		{
			for (uint i = 0u; i < m_propNodes.getCount(); ++i)
			{
				TreePropNode& propNode = m_propNodes[ i ];

				if ( propNode.nodeLayout.getRectangle().contains( state.mousePosition ) )
				{
					m_selectedProp = i;

					return true;
				}
			}
		}

		return DebugGuiWindow::processInputEvent( inputEvent, state );
	}

	bool DebugGuiWindowDebugProp::processGuiEvent( const DebugGuiEvent& guiEvent )
	{
		if ( guiEvent.eventType == DebugGuiEventType_Click )
		{
			for (uint parentIndex = 0u; parentIndex < m_folderNodes.getCount(); ++parentIndex)
			{
				TreeFolderNode& parentNode = m_folderNodes[ parentIndex ];

				if ( guiEvent.pControl == &parentNode.expandButton )
				{
					if ( parentNode.chilrenLayout.getChildCount() == 0 )
					{
						// expand
						parentNode.expandButton.setText( "-" );

						for (uint i = 0u; i < m_folderNodes.getCount(); ++i)
						{
							TreeFolderNode& node = m_folderNodes[ i ];

							if ( node.parentIndex == parentIndex )
							{
								parentNode.chilrenLayout.addChildControl( &node.fullLayout );
							}
						}

						for (uint i = 0u; i < m_propNodes.getCount(); ++i)
						{
							TreePropNode& node = m_propNodes[ i ];

							if ( node.parentIndex == parentIndex )
							{
								parentNode.chilrenLayout.addChildControl( &node.nodeLayout );
							}
						}
					}
					else
					{
						// collapse
						parentNode.expandButton.setText( "+" );

						parentNode.chilrenLayout.dispose();
						parentNode.chilrenLayout.create();
					}

					refreshRectangle();
				}
			}
		}

		return DebugGuiWindow::processGuiEvent( guiEvent );
	}

	void DebugGuiWindowDebugProp::setLayoutParameters( DebugGuiHorizontalLayout& layout )
	{
		layout.setPadding( Thickness() );
		layout.setExpandChildren( false );
	}

	void DebugGuiWindowDebugProp::setLayoutParameters( DebugGuiVerticalLayout& layout )
	{
		layout.setPadding( Thickness() );
		layout.setExpandChildren( false );
	}

	void DebugGuiWindowDebugProp::setDebugPropText( DebugGuiLabel& targetLabel, const DebugProp& prop )
	{
		switch (prop.getType())
		{
		case DebugProp::Type_Bool:
			{
				const DebugPropBool& propBool = static_cast< const DebugPropBool& >( prop );

				const char* pText = ( propBool.getValue() ? "true" : "false" );
				targetLabel.setText( pText );
			}
			break;

		case DebugProp::Type_Float:
			{
				const DebugPropFloat& propFloat = static_cast< const DebugPropFloat& >( prop );

				char buffer[ 128u ];
				formatStringBuffer( buffer, TIKI_COUNT( buffer ), "%f", propFloat.getValue() );
				targetLabel.setText( buffer );
			}
			break;

		case DebugProp::Type_Int:
			{
				const DebugPropInt& propInt = static_cast< const DebugPropInt& >( prop );

				char buffer[ 128u ];
				formatStringBuffer( buffer, TIKI_COUNT( buffer ), "%i", propInt.getValue() );
				targetLabel.setText( buffer );
			}
			break;

		default:
			TIKI_ASSERT(false);
			break;
		}
	}
}