#pragma once
#ifndef TIKI_UISYSTEM_HPP_INCLUDED
#define TIKI_UISYSTEM_HPP_INCLUDED

#include "tiki/base/types.hpp"
#include "tiki/container/linkedlist.hpp"
#include "tiki/container/pool.hpp"
#include "tiki/script/scriptcontext.hpp"
#include "tiki/ui/uielementscript.hpp"
#include "tiki/ui/uirenderer.hpp"
#include "tiki/ui/uisystemscript.hpp"
#include "tiki/ui/uitypes.hpp"

namespace tiki
{
	class GraphicsContext;
	class GraphicsSystem;
	class RenderTarget;
	class ResourceManager;
	struct InputEvent;
	struct UiEventHandler;
	
	struct UiSystemParameters
	{
		UiSystemParameters()
		{
			width					= 0u;
			height					= 0u;

			maxElementCount			= 4096u;
			maxEventHandlerCount	= 4096u;
		}

		uint	width;
		uint	height;

		uint	maxElementCount;
		uint	maxEventHandlerCount;
	};

	class UiSystem
	{
		TIKI_NONCOPYABLE_CLASS( UiSystem );
		friend class UiElement;

	public:

						UiSystem();
						~UiSystem();

		bool			create( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager, const UiSystemParameters& parameters );
		void			dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager );

		UiElement*		addElement( UiElement* pParent = nullptr, crc32 elementTypeCrc = TIKI_INVALID_CRC32 );
		void			removeElement( UiElement* pElement );

		void			update();
		void			render( GraphicsContext& context, const RenderTarget& renderTarget ) const;

		ScriptContext&	getScriptContext();

		void			setScreenSize( float width, float height );

		bool			processInputEvent( InputEvent& inputEvent );

	private: // friend

		UiEventHandler*	allocateEventHandler();
		void			freeEventHandler( UiEventHandler& eventHandler );

	private:

		UiRenderer				m_renderer;

		ScriptContext			m_scriptContext;
		UiSystemScript			m_scriptUiSystemClass;
		UiElementScript			m_scriptUiElementClass;

		Pool< UiElement >		m_elementPool;
		UiElement*				m_pRootElement;

		Pool< UiEventHandler >	m_eventHandlerPool;

		UiInputState			m_prevInputState;
		UiInputState			m_currentInputState;

	};
}

#endif // TIKI_UISYSTEM_HPP_INCLUDED
