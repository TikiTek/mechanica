#pragma once
#ifndef TIKI_UISYSTEM_HPP_INCLUDED
#define TIKI_UISYSTEM_HPP_INCLUDED

#include "tiki/base/types.hpp"
#include "tiki/container/linkedlist.hpp"
#include "tiki/container/pool.hpp"
#include "tiki/script/scriptcontext.hpp"
#include "tiki/ui/uirenderer.hpp"

namespace tiki
{
	class GraphicsContext;
	class GraphicsSystem;
	class RenderTarget;
	class ResourceManager;
	struct InputEvent;
	struct UiSize;

	struct UiSystemParameters
	{
		UiSystemParameters()
		{
			width			= 0u;
			height			= 0u;

			maxElementCount	= 4096u;
		}

		uint	width;
		uint	height;

		uint	maxElementCount;
	};

	class UiSystem
	{
		TIKI_NONCOPYABLE_CLASS( UiSystem );

	public:

						UiSystem();
						~UiSystem();

		bool			create( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager, const UiSystemParameters& parameters );
		void			dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager );

		UiElement*		addElement( UiElement* pParent = nullptr );
		void			removeElement( UiElement* pElement );

		void			update();
		void			render( GraphicsContext& context, const RenderTarget& renderTarget ) const;

		void			setScreenSize( float width, float height );

		bool			processInputEvent( InputEvent& inputEvent );

	private:

		UiRenderer				m_renderer;

		ScriptContext			m_scriptContext;

		Pool< UiElement >		m_elementPool;
		UiElement*				m_pRootElement;

	};
}

#endif // TIKI_UISYSTEM_HPP_INCLUDED
